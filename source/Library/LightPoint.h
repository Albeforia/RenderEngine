#pragma once

#include "Common.h"
#include "Light.h"

namespace Library {

	class LightPoint : public Light {

		RTTI_DECLARATIONS(LightPoint, Light)

	public:
		LightPoint(Game&);
		virtual ~LightPoint();

		XMFLOAT3& position();
		XMVECTOR positionv() const;
		float radius() const;

		virtual void set_position(float x, float y, float z);
		virtual void set_position(FXMVECTOR);
		virtual void set_position(const XMFLOAT3&);

		XMVECTOR attenuation() const;
		virtual void set_attenuation(float constant, float linear, float quadratic);

		virtual void set_color(FLOAT r, FLOAT g, FLOAT b, FLOAT a) override;
		virtual void set_color(XMCOLOR) override;
		virtual void set_color(FXMVECTOR) override;
		virtual void set_intensity(float) override;

	protected:
		void update_radius();

		XMFLOAT3 m_position;
		float m_radius;
		float m_constant;
		float m_linear;
		float m_quadratic;

	};

}