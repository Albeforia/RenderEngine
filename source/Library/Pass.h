#pragma once

#include "Common.h"

namespace Library {

	class Technique;
	class Game;

	/**
	 *  Wrapper around ID3DX11EffectPass.
	 *  Pass owns ID3D11InputLayout.
	 */
	class Pass {

	public:
		Pass(Game&, Technique&, ID3DX11EffectPass*);
		~Pass();

		const std::string& name() const;
		ID3D11InputLayout* input_layout() const;

		void apply(UINT flags, ID3D11DeviceContext*) const;

	private:
		HRESULT create_input_layout(const Game&);

		Technique& m_technique;

		ID3DX11EffectPass* m_pass;
		D3DX11_PASS_DESC m_pass_desc;
		ID3D11InputLayout* m_input_layout;

		std::string m_name;

	};

}