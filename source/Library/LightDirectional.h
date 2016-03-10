#pragma once

#include "Common.h"
#include "Light.h"

namespace Library {

	class LightDirectional : public Light {

		RTTI_DECLARATIONS(LightDirectional, Light);

	public:
		LightDirectional(Game&);
		virtual ~LightDirectional();

		const XMFLOAT3& direction() const;
		const XMFLOAT3& up() const;
		const XMFLOAT3& right() const;
		XMVECTOR directionv() const;
		XMVECTOR upv() const;
		XMVECTOR rightv() const;

		void apply_rotation(CXMMATRIX transform);
		void apply_rotation(const XMFLOAT4X4& transform);

	protected:
		XMFLOAT3 m_direction;
		XMFLOAT3 m_up;
		XMFLOAT3 m_right;
	};

}