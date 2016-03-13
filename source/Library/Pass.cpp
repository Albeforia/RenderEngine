#include <d3dcompiler.inl>
#include "Pass.h"
#include "Game.h"
#include "GameException.h"

namespace Library {

	Pass::Pass(Game& game, Technique& technique, ID3DX11EffectPass* pass)
		: m_technique {technique}, m_pass {pass}, m_pass_desc {},
		m_input_layout {}, m_layout_descs {}, m_name {} {
		m_pass->GetDesc(&m_pass_desc);
		m_name = m_pass_desc.Name;
		if (FAILED(create_input_layout(game))) {
			throw GameException("ID3D11Device::CreateInputLayout() failed.");
		}
	}

	Pass::~Pass() {
		ReleaseObject(m_input_layout);
		ReleaseObject(m_pass);
	}

	const std::string& Pass::name() const {
		return m_name;
	}

	ID3D11InputLayout* Pass::input_layout() const {
		return m_input_layout;
	}

	void Pass::apply(UINT flags, ID3D11DeviceContext* con) const {
		m_pass->Apply(flags, con);
	}

	HRESULT Pass::create_input_layout(const Game& game) {
		D3DX11_PASS_SHADER_DESC psd;
		m_pass->GetVertexShaderDesc(&psd);
		ID3DX11EffectShaderVariable* sv = psd.pShaderVariable->AsShader();
		D3DX11_EFFECT_SHADER_DESC esd;
		sv->GetShaderDesc(0, &esd);
		ReleaseObject(sv);
		// reflect
		ID3D11ShaderReflection* shader_ref = nullptr;
		if (D3D11Reflect(esd.pBytecode, esd.BytecodeLength, &shader_ref)) {
			throw GameException("D3D11Reflect() failed.");
		}
		D3D11_SHADER_DESC sd;
		shader_ref->GetDesc(&sd);

		// read input layout description
		m_layout_descs.reserve(sd.InputParameters);
		for (UINT i = 0; i < sd.InputParameters; i++) {
			D3D11_SIGNATURE_PARAMETER_DESC pd;
			shader_ref->GetInputParameterDesc(i, &pd);
			// fill out input element desc
			D3D11_INPUT_ELEMENT_DESC ed;
			ed.SemanticName = pd.SemanticName;
			ed.SemanticIndex = pd.SemanticIndex;
			ed.InputSlot = 0;
			ed.AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
			ed.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ed.InstanceDataStepRate = 0;
			// instancing check
			std::string s(ed.SemanticName);
			if (s.substr(0, 5) == "INST_") {
				ed.InputSlot = 1;
				ed.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				ed.InstanceDataStepRate = 1;
			}
			// determine DXGI format
			if (pd.Mask == 1) {
				if (pd.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ed.Format = DXGI_FORMAT_R32_UINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ed.Format = DXGI_FORMAT_R32_SINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ed.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (pd.Mask <= 3) {
				if (pd.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ed.Format = DXGI_FORMAT_R32G32_UINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ed.Format = DXGI_FORMAT_R32G32_SINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ed.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (pd.Mask <= 7) {
				if (pd.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ed.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ed.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ed.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (pd.Mask <= 15) {
				if (pd.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ed.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ed.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (pd.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ed.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			m_layout_descs.push_back(ed);
		}

		HRESULT hr = game.d3d_device()->CreateInputLayout(&m_layout_descs[0], m_layout_descs.size(),
														  m_pass_desc.pIAInputSignature,
														  m_pass_desc.IAInputSignatureSize,
														  &m_input_layout);
		ReleaseObject(shader_ref);
		return hr;
	}

}