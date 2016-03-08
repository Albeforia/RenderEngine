#include "Mouse.h"
#include "Game.h"
#include "GameTime.h"
#include "GameException.h"

namespace Library {

	RTTI_DEFINITIONS(Mouse);

	Mouse::Mouse(Game& game, LPDIRECTINPUT8 input)
		: GameComponent(game), m_input {input}, m_device {}, m_x {}, m_y {}, m_wheel {} {
		assert(m_input != nullptr);
		ZeroMemory(&m_curr_state, sizeof(m_curr_state));
		ZeroMemory(&m_last_state, sizeof(m_last_state));
	}

	Mouse::~Mouse() {
		if (m_device != nullptr) {
			m_device->Unacquire();
			m_device->Release();
			m_device = nullptr;
		}
	}

	LPDIMOUSESTATE  Mouse::curr_state() { return &m_curr_state; }
	LPDIMOUSESTATE  Mouse::last_state() { return &m_last_state; }

	long Mouse::x() const { return m_x; }
	long Mouse::y() const { return m_y; }
	long Mouse::wheel()const { return m_wheel; }

	void Mouse::init() {
		if (FAILED(m_input->CreateDevice(GUID_SysMouse, &m_device, nullptr))) {
			throw GameException("LPDIRECTINPUT8::CreateDevice() failed.");
		}
		if (FAILED(m_device->SetDataFormat(&c_dfDIMouse))) {
			throw GameException("LPDIRECTINPUTDEVICE8::SetDataFormat() failed.");
		}
		if (FAILED(m_device->SetCooperativeLevel(m_game->window_handle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
			throw GameException("LPDIRECTINPUTDEVICE8::SetCooperativeLevel() failed.");
		}
		if (FAILED(m_device->Acquire())) {
			throw GameException("LPDIRECTINPUTDEVICE8::Acquire() failed.");
		}
	}

	void Mouse::update(const GameTime& game_time) {
		if (m_device != nullptr) {
			memcpy(&m_last_state, &m_curr_state, sizeof(m_curr_state));
			if (FAILED(m_device->GetDeviceState(sizeof(m_curr_state), &m_curr_state))) {
				if (SUCCEEDED(m_device->Acquire())) {
					if (FAILED(m_device->GetDeviceState(sizeof(m_curr_state), &m_curr_state))) {
						return;
					}
				}
			}
			m_x += m_curr_state.lX;
			m_y += m_curr_state.lY;
			m_wheel += m_curr_state.lZ;
		}
	}

	bool Mouse::button_up(MouseButtons button) const {
		return (m_curr_state.rgbButtons[button] & 0x80) == 0;
	}
	bool Mouse::button_down(MouseButtons button) const {
		return (m_curr_state.rgbButtons[button] & 0x80) != 0;
	}
	bool Mouse::button_uped(MouseButtons button) const {
		return (m_last_state.rgbButtons[button] & 0x80) == 0;
	}
	bool Mouse::button_downed(MouseButtons button) const {
		return (m_last_state.rgbButtons[button] & 0x80) != 0;
	}
	bool Mouse::button_pressed(MouseButtons button) const {
		return button_uped(button) && button_down(button);
	}
	bool Mouse::button_released(MouseButtons button) const {
		return button_downed(button) && button_up(button);
	}
	bool Mouse::button_held(MouseButtons button) const {
		return button_downed(button) && button_down(button);
	}

}