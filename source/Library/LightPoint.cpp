#include "LightPoint.h"
#include "VectorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(LightPoint);

	LightPoint::LightPoint(Game& game)
		: Light(game), m_position {Vector3Helper::Zero}, m_radius {1.0f} {}

	LightPoint::~LightPoint() {}

	XMFLOAT3& LightPoint::position() {
		return m_position;
	}

	XMVECTOR LightPoint::positionv() const {
		return XMLoadFloat3(&m_position);
	}

	float LightPoint::radius() const {
		return m_radius;
	}

	void LightPoint::set_position(float x, float y, float z) {
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		set_position(position);
	}

	void LightPoint::set_position(FXMVECTOR position) {
		XMStoreFloat3(&m_position, position);
	}

	void LightPoint::set_position(const XMFLOAT3& position) {
		m_position = position;
	}

	XMVECTOR LightPoint::attenuation() const {
		XMFLOAT3 a(m_constant, m_linear, m_quadratic);
		return XMLoadFloat3(&a);
	}

	void LightPoint::set_attenuation(float constant, float linear, float quadratic) {
		m_constant = constant;
		m_linear = linear;
		m_quadratic = quadratic;
		update_radius();
	}

	void LightPoint::set_color(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {
		Light::set_color(r, g, b, a);
		update_radius();
	}
	void LightPoint::set_color(XMCOLOR c) {
		Light::set_color(c);
		update_radius();
	}
	void LightPoint::set_color(FXMVECTOR c) {
		Light::set_color(c);
		update_radius();
	}
	void LightPoint::set_intensity(float i) {
		Light::set_intensity(i);
		update_radius();
	}

	void LightPoint::update_radius() {
		XMVECTOR v = colorv();
		float r = XMVectorGetX(v);
		float g = XMVectorGetY(v);
		float b = XMVectorGetZ(v);
		float max = std::fmaxf(std::fmaxf(r, g), b);
		m_radius = (-m_linear +
					std::sqrtf(m_linear * m_linear -
							   4 * m_quadratic * (m_constant - 256.0f * intensity() * max)))
			/ (2 * m_quadratic);
	}

}