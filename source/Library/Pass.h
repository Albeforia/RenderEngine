#pragma once

#include "Common.h"
#include "Game.h"

namespace Library {

	class Technique;

	class Pass {

	public:
		Pass(Game&, Technique&, ID3DX11EffectPass*);
		~Pass();
		Pass(const Pass&) = delete;
		Pass& operator=(const Pass&) = delete;

		Technique& technique();
		ID3DX11EffectPass* pass() const;
		const D3DX11_PASS_DESC& pass_desc() const;
		const std::string& name() const;

		void create_input_layout(const D3D11_INPUT_ELEMENT_DESC*, UINT num, ID3D11InputLayout**);
		void apply(UINT flags, ID3D11DeviceContext*);

	private:
		Game& m_game;
		Technique& m_technique;
		ID3DX11EffectPass* m_pass;
		D3DX11_PASS_DESC m_pass_desc;
		std::string m_name;

	};

}
