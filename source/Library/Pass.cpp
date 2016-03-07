#include "Pass.h"
#include "GameException.h"

namespace Library {

	Pass::Pass(Game& game, Technique& technique, ID3DX11EffectPass* pass)
		: m_game {game}, m_technique {technique}, m_pass {pass},
		m_pass_desc {}, m_name {} {
		m_pass->GetDesc(&m_pass_desc);
		m_name = m_pass_desc.Name;
	}

	void Pass::create_input_layout(const D3D11_INPUT_ELEMENT_DESC* desc, UINT num, ID3D11InputLayout** layout) {
		HRESULT hr = m_game.d3d_device()->CreateInputLayout(desc, num, m_pass_desc.pIAInputSignature,
															m_pass_desc.IAInputSignatureSize, layout);
		if (FAILED(hr)) {
			throw GameException("ID3D11Device1::CreateInputLayout() failed.", hr);
		}
	}

	void Pass::apply(UINT flags, ID3D11DeviceContext* con) {
		m_pass->Apply(flags, con);
	}

}