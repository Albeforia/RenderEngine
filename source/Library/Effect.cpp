#include <d3dcompiler.h>
#include "Effect.h"
#include "GameException.h"
#include "Utility.h"

namespace Library {

	void Effect::compile_from_file(ID3D11Device* dev, ID3DX11Effect** effect, const std::wstring& file) {
		UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3D10Blob* compiled = nullptr;
		ID3D10Blob* err_msg = nullptr;
		HRESULT hr = D3DCompileFromFile(file.c_str(), nullptr, nullptr, nullptr,
										"fx_5_0", flags, 0, &compiled, &err_msg);
		if (err_msg != nullptr) {
			GameException e(reinterpret_cast<char*>(err_msg->GetBufferPointer()), hr);
			throw e;
		}
		if (FAILED(hr)) {
			throw GameException("D3DCompileFromFile() failed.", hr);
		}
		if (FAILED(hr = D3DX11CreateEffectFromMemory(compiled->GetBufferPointer(), compiled->GetBufferSize(),
													 0, dev, effect))) {
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}
		ReleaseObject(compiled);
	}

	void Effect::compile_from_file(const std::wstring& file) {
		compile_from_file(m_game.d3d_device(), &m_effect, file);
	}

	void Effect::load(ID3D11Device* dev, ID3DX11Effect** effect, const std::wstring& file) {
		std::vector<char> compiled;
		Utility::LoadBinaryFile(file, compiled);
		HRESULT hr = D3DX11CreateEffectFromMemory(&compiled.front(), compiled.size(), 0, dev, effect);
		if (FAILED(hr)) {
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	void Effect::load(const std::wstring& file) {
		load(m_game.d3d_device(), &m_effect, file);
	}

	Effect::Effect(Game& game)
		: m_game {game} {}

	Effect::~Effect() {};

	Game&Effect::game() { return m_game; }
	ID3DX11Effect*Effect::effect() const { return m_effect; }
	const D3DX11_EFFECT_DESC&Effect::effect_desc() const { return m_effect_desc; }
	const std::vector<Technique*>&Effect::techniques() const { return m_techniques; }
	const std::map<std::string, Technique*>&Effect::techniques_by_name() const { return m_techniques_by_name; }
	const std::vector<Variable*>&Effect::variables() const { return m_variables; }
	const std::map<std::string, Variable*>&Effect::variables_by_name() const { return m_variables_by_name; }

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

}