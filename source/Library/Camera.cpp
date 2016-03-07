#include "Camera.h"
#include "Game.h"
#include "GameTime.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"

namespace Library {

	RTTI_DEFINITIONS(Camera);

	const float Camera::DEFAULT_FOV = XM_PIDIV4;
	const float Camera::DEFAULT_NEARP = 0.01f;
	const float Camera::DEFAULT_FARP = 1000.0f;

	Camera::Camera(Game& game)
		: GameComponent(game),
		m_fov {DEFAULT_FOV}, m_ratio {game.aspect_ratio()}, m_nearp {DEFAULT_NEARP}, m_farp {DEFAULT_FARP},
		m_position {}, m_direction {}, m_up {}, m_right {}, m_view {}, m_projection {} {}

	Camera::Camera(Game& game, float fov, float ratio, float nearp, float farp)
		: Camera(game) {
		m_fov = fov;
		m_ratio = ratio;
		m_nearp = nearp;
		m_farp = farp;
	}

	Camera::~Camera() {}

	XMMATRIX Camera::view_projection() const {
		XMMATRIX v = XMLoadFloat4x4(&m_view);
		XMMATRIX p = XMLoadFloat4x4(&m_projection);
		return XMMatrixMultiply(v, p);
	}

	void Camera::set_position(FLOAT x, FLOAT y, FLOAT z) {
		XMVECTOR p = XMVectorSet(x, y, z, 1.0f);
		set_position(p);
	}

	void Camera::set_position(FXMVECTOR position) {
		XMStoreFloat3(&m_position, position);
	}

	void Camera::set_position(const XMFLOAT3& position) {
		m_position = position;
	}

	void Camera::reset() {
		m_position = Vector3Helper::Zero;
		m_direction = Vector3Helper::Forward;
		m_up = Vector3Helper::Up;
		m_right = Vector3Helper::Right;
		update_view();
	}

	void Camera::init() {
		update_projection();
		reset();
	}

	void Camera::update(const GameTime& game_time) {
		// TODO camera moved dirty flag
		update_view();
	}

	void Camera::update_view() {
		XMVECTOR eye = XMLoadFloat3(&m_position);
		XMVECTOR look = XMLoadFloat3(&m_direction);
		XMVECTOR up = XMLoadFloat3(&m_up);
		XMMATRIX view = XMMatrixLookToRH(eye, look, up);
		XMStoreFloat4x4(&m_view, view);
	}

	void Camera::update_projection() {
		XMMATRIX projection = XMMatrixPerspectiveFovRH(m_fov, m_ratio, m_nearp, m_farp);
		XMStoreFloat4x4(&m_projection, projection);
	}

	void Camera::apply_rotation(CXMMATRIX transform) {
		XMVECTOR dir = XMLoadFloat3(&m_direction);
		XMVECTOR up = XMLoadFloat3(&m_up);
		dir = XMVector3TransformNormal(dir, transform);
		dir = XMVector3Normalize(dir);
		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);
		XMVECTOR right = XMVector3Cross(dir, up);
		// the 2nd cross gurantee orthogonal
		up = XMVector3Cross(right, dir);
		XMStoreFloat3(&m_direction, dir);
		XMStoreFloat3(&m_up, up);
		XMStoreFloat3(&m_right, right);
	}

	void Camera::apply_rotation(const XMFLOAT4X4& transform) {
		XMMATRIX trans = XMLoadFloat4x4(&transform);
		apply_rotation(trans);
	}

}