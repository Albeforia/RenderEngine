#include <d3dcompiler.h>
#include "Effect.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

namespace Library {

	// static
	void Effect::compile_from_file(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& file) {
		UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3D10Blob* compiled = nullptr;
		ID3D10Blob* err_msg = nullptr;
		HRESULT hr = D3DCompileFromFile(file.c_str(), nullptr, nullptr, nullptr,
										"fx_5_0", flags, 0, &compiled, &err_msg);
		if (err_msg != nullptr) {
			throw GameException(reinterpret_cast<char*>(err_msg->GetBufferPointer()), hr);
		}
		if (FAILED(hr)) {
			throw GameException("D3DCompileFromFile() failed.", hr);
		}
		if (FAILED(hr = D3DX11CreateEffectFromMemory(compiled->GetBufferPointer(), compiled->GetBufferSize(),
													 0, device, effect))) {
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}
		ReleaseObject(compiled);
	}

	// static
	void Effect::load(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& file) {
		std::vector<char> compiled;
		Utility::LoadBinaryFile(file, compiled);
		HRESULT hr = D3DX11CreateEffectFromMemory(&compiled.front(), compiled.size(), 0, device, effect);
		if (FAILED(hr)) {
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	void Effect::compile_from_file(const std::wstring& file) {
		compile_from_file(m_game.d3d_device(), &m_effect, file);
		init();
	}

	void Effect::load(const std::wstring& file) {
		load(m_game.d3d_device(), &m_effect, file);
		init();
	}

	Effect::Effect(Game& game)
		: m_game {game} {}

	Effect::~Effect() {
		for (auto* t : m_techniques) {
			DeleteObject(t);
		}
		for (auto* v : m_variables) {
			DeleteObject(v);
		}
		ReleaseObject(m_effect);
	};

	void Effect::init() {
		HRESULT hr = m_effect->GetDesc(&m_effect_desc);
		if (FAILED(hr)) {
			throw GameException("ID3DX11Effect::GetDesc() failed.", hr);
		}
		for (UINT i = 0; i < m_effect_desc.Techniques; i++) {
			auto* t = new Technique(m_game, *this, m_effect->GetTechniqueByIndex(i));
			m_techniques.push_back(t);
			m_techniques_by_name.insert({t->name(), t});
		}
		for (UINT i = 0; i < m_effect_desc.GlobalVariables; i++) {
			auto* v = new Variable(*this, m_effect->GetVariableByIndex(i));
			m_variables.push_back(v);
			m_variables_by_name.insert({v->name(), v});
		}
	}

	const Technique* Effect::technique(int index) const {
		return m_techniques.at(index);
	}

	const Technique* Effect::technique(const std::string& name) const {
		return m_techniques_by_name.at(name);
	}

	Variable* Effect::variable(int index) const {
		return m_variables.at(index);
	}

	Variable* Effect::variable(const std::string& name) const {
		return m_variables_by_name.at(name);
	}

}