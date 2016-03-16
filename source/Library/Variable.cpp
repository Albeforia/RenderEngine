#include "Variable.h"
#include "GameException.h"

namespace Library {

	Variable::Variable(Effect& effect, ID3DX11EffectVariable* variable)
		: m_effect {effect}, m_variable {variable}, m_variable_desc {}, m_name {} {
		m_variable->GetDesc(&m_variable_desc);
		m_name = m_variable_desc.Name;
		m_variable_type = m_variable->GetType();
		m_variable_type->GetDesc(&m_variable_type_desc);
	}

	Variable::~Variable() {
		ReleaseObject(m_variable_type);
		ReleaseObject(m_variable);
	}

	const std::string& Variable::name() const {
		return m_name;
	}

	const D3DX11_EFFECT_TYPE_DESC& Variable::type_desc() const {
		return m_variable_type_desc;
	}

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

	Variable& Variable::operator<<(const std::vector<float>& values) {
		ID3DX11EffectScalarVariable* variable = m_variable->AsScalar();
		if (variable->IsValid() == false) {
			throw GameException("Invalid effect variable cast.");
		}

		variable->SetFloatArray(&values[0], 0, values.size());

		return *this;
	}

	Variable& Variable::operator<<(const std::vector<XMFLOAT2>& values) {
		ID3DX11EffectVectorVariable* variable = m_variable->AsVector();
		if (variable->IsValid() == false) {
			throw GameException("Invalid effect variable cast.");
		}

		variable->SetFloatVectorArray(reinterpret_cast<const float*>(&values[0]), 0, values.size());

		return *this;
	}

}