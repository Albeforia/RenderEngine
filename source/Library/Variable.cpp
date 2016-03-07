#include "Variable.h"
#include "GameException.h"

namespace Library {

	Variable::Variable(Effect& effect, ID3DX11EffectVariable* variable)
		: m_effect {effect}, m_variable {variable}, m_variable_desc {},
		m_type {}, m_type_desc {}, m_name {} {}

	Effect&Variable::effect() { return m_effect; }
	ID3DX11EffectVariable*Variable::variable() const { return m_variable; }
	const D3DX11_EFFECT_VARIABLE_DESC&Variable::variable_desc() const { return m_variable_desc; }
	ID3DX11EffectType*Variable::type() const { return m_type; }
	const D3DX11_EFFECT_TYPE_DESC&Variable::type_desc() const { return m_type_desc; }
	const std::string Variable::name() const { return m_name; }

	Variable& Variable::operator<<(CXMMATRIX value) {
		auto* v = m_variable->AsMatrix();
		if (!v->IsValid()) {
			throw GameException("Invalid effect variable cast.");
		}
		v->SetMatrix(reinterpret_cast<const float*>(&value));
		return *this;
	}

	Variable& Variable::operator<<(ID3D11ShaderResourceView* value) {
		auto* v = m_variable->AsShaderResource();
		if (!v->IsValid()) {
			throw GameException("Invalid effect variable cast.");
		}
		v->SetResource(value);
		return *this;
	}

	Variable& Variable::operator<<(FXMVECTOR value) {
		auto* v = m_variable->AsVector();
		if (!v->IsValid()) {
			throw GameException("Invalid effect variable cast.");
		}
		v->SetFloatVector(reinterpret_cast<const float*>(&value));
		return *this;
	}

	Variable& Variable::operator<<(float value) {
		auto* v = m_variable->AsScalar();
		if (!v->IsValid()) {
			throw GameException("Invalid effect variable cast.");
		}
		v->SetFloat(value);
		return *this;
	}

}