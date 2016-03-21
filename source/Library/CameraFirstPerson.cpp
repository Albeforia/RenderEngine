#include "CameraFirstPerson.h"
#include "Game.h"
#include "GameTime.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "VectorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(CameraFirstPerson);

	const float CameraFirstPerson::DEFAULT_MOUSE_SENSITIVITY = 100.0f;
	const float CameraFirstPerson::DEFAULT_ROTATION_RATE = XMConvertToRadians(1.0f);
	const float CameraFirstPerson::DEFAULT_MOVEMENT_RATE = 10.0f;

	CameraFirstPerson::CameraFirstPerson(Game& game)
		: CameraPerspective(game), m_keyboard {}, m_mouse {},
		m_mouse_sensitivity {DEFAULT_MOUSE_SENSITIVITY},
		m_rotation_rate {DEFAULT_ROTATION_RATE},
		m_movement_rate {DEFAULT_MOVEMENT_RATE} {}

	CameraFirstPerson::CameraFirstPerson(Game& game, float fov, float ratio,
										 float nearp, float farp,
										 float focus_distance, float focus_range)
		: CameraPerspective(game, fov, ratio, nearp, farp, focus_distance, focus_range),
		m_keyboard {}, m_mouse {},
		m_mouse_sensitivity {DEFAULT_MOUSE_SENSITIVITY},
		m_rotation_rate {DEFAULT_ROTATION_RATE},
		m_movement_rate {DEFAULT_MOVEMENT_RATE} {}

	CameraFirstPerson::~CameraFirstPerson() {
		m_keyboard = nullptr;
		m_mouse = nullptr;
	}

	const Keyboard& CameraFirstPerson::keyboard() const {
		return *m_keyboard;
	}

	void CameraFirstPerson::set_keyboard(Keyboard& keyboard) {
		m_keyboard = &keyboard;
	}

	const Mouse& CameraFirstPerson::mouse() const {
		return *m_mouse;
	}

	void CameraFirstPerson::set_mouse(Mouse& mouse) {
		m_mouse = &mouse;
	}

	float& CameraFirstPerson::mouse_sensitivity() {
		return m_mouse_sensitivity;
	}

	float& CameraFirstPerson::rotation_rate() {
		return m_rotation_rate;
	}

	float& CameraFirstPerson::movement_rate() {
		return m_movement_rate;
	}

	void CameraFirstPerson::init() {
		m_keyboard = (Keyboard*)m_game->services().get_service(Keyboard::TypeIdClass());
		m_mouse = (Mouse*)m_game->services().get_service(Mouse::TypeIdClass());
		CameraPerspective::init();
	}

	void CameraFirstPerson::update(const GameTime& game_time) {
		XMFLOAT2 movement_amount = Vector2Helper::Zero;
		if (m_keyboard != nullptr) {
			if (m_keyboard->key_down(DIK_W)) {
				movement_amount.y = 1.0f;
			}

			if (m_keyboard->key_down(DIK_S)) {
				movement_amount.y = -1.0f;
			}

			if (m_keyboard->key_down(DIK_A)) {
				movement_amount.x = -1.0f;
			}

			if (m_keyboard->key_down(DIK_D)) {
				movement_amount.x = 1.0f;
			}
		}

		XMFLOAT2 rotation_amount = Vector2Helper::Zero;
		if ((m_mouse != nullptr) && (m_mouse->button_held(MouseButtonLeft))) {
			LPDIMOUSESTATE mouseState = m_mouse->curr_state();
			rotation_amount.x = -mouseState->lX * m_mouse_sensitivity;
			rotation_amount.y = -mouseState->lY * m_mouse_sensitivity;
		}

		float delta = (float)game_time.elapsed_game_time();
		XMVECTOR rotation_vector = XMLoadFloat2(&rotation_amount) * m_rotation_rate * delta;
		XMVECTOR right = XMLoadFloat3(&m_right);

		XMMATRIX pitch_mat = XMMatrixRotationAxis(right, XMVectorGetY(rotation_vector));
		XMMATRIX yaw_mat = XMMatrixRotationY(XMVectorGetX(rotation_vector));

		apply_rotation(XMMatrixMultiply(pitch_mat, yaw_mat));

		XMVECTOR position = XMLoadFloat3(&m_position);
		XMVECTOR movement = XMLoadFloat2(&movement_amount) * m_movement_rate * delta;

		XMVECTOR strafe = right * XMVectorGetX(movement);
		position += strafe;

		XMVECTOR forward = XMLoadFloat3(&m_direction) * XMVectorGetY(movement);
		position += forward;

		XMStoreFloat3(&m_position, position);

		CameraPerspective::update(game_time);
	}

}