#include "LightDirectional.h"
#include "VectorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(LightDirectional);

	LightDirectional::LightDirectional(Game& game)
		: Light(game),
		m_direction {Vector3Helper::Forward}, m_up {Vector3Helper::Up}, m_right {Vector3Helper::Right} {}

	LightDirectional::~LightDirectional() {}

	const XMFLOAT3& LightDirectional::direction() const {
		return m_direction;
	}

	const XMFLOAT3& LightDirectional::up() const {
		return m_up;
	}

	const XMFLOAT3& LightDirectional::right() const {
		return m_right;
	}

	XMVECTOR LightDirectional::directionv() const {
		return XMLoadFloat3(&m_direction);
	}

	XMVECTOR LightDirectional::upv() const {
		return XMLoadFloat3(&m_up);
	}

	XMVECTOR LightDirectional::rightv() const {
		return XMLoadFloat3(&m_right);
	}

	void LightDirectional::apply_rotation(CXMMATRIX transform) {
		XMVECTOR direction = XMLoadFloat3(&m_direction);
		XMVECTOR up = XMLoadFloat3(&m_up);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&m_direction, direction);
		XMStoreFloat3(&m_up, up);
		XMStoreFloat3(&m_right, right);
	}

	void LightDirectional::apply_rotation(const XMFLOAT4X4& transform) {
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		apply_rotation(transformMatrix);
	}

}