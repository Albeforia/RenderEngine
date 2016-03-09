#pragma once

#include "Common.h"

namespace Library {

	class Effect;

	class Variable {

	public:
		Variable(Effect&, ID3DX11EffectVariable*);
		~Variable();
		Variable(const Variable&) = delete;
		Variable& operator=(const Variable&) = delete;

		Effect& effect();
		ID3DX11EffectVariable* variable() const;
		const D3DX11_EFFECT_VARIABLE_DESC& variable_desc() const;
		ID3DX11EffectType* type() const;
		//const D3DX11_EFFECT_TYPE_DESC& type_desc() const;
		const std::string name() const;

		Variable& operator<<(CXMMATRIX);
		Variable& operator<<(ID3D11ShaderResourceView*);
		Variable& operator<<(FXMVECTOR);
		Variable& operator<<(float);

	private:
		Effect& m_effect;
		ID3DX11EffectVariable* m_variable;
		D3DX11_EFFECT_VARIABLE_DESC m_variable_desc;
		ID3DX11EffectType* m_type;
		//D3DX11_EFFECT_TYPE_DESC m_type_desc;
		std::string m_name;

	};

}