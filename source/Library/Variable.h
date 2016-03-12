#pragma once

#include "Common.h"

namespace Library {

	class Effect;

	/**
	 *  Wrapper around ID3DX11EffectVariable.
	 */
	class Variable {

	public:
		Variable(Effect&, ID3DX11EffectVariable*);
		~Variable();

		const std::string& name() const;

		Variable& operator<<(CXMMATRIX);
		Variable& operator<<(ID3D11ShaderResourceView*);
		Variable& operator<<(FXMVECTOR);
		Variable& operator<<(float);

	private:
		Effect& m_effect;

		ID3DX11EffectVariable* m_variable;
		D3DX11_EFFECT_VARIABLE_DESC m_variable_desc;

		std::string m_name;

	};

}