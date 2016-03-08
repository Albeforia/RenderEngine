#include "Keyboard.h"
#include "Game.h"
#include "GameTime.h"
#include "GameException.h"

namespace Library {

	RTTI_DEFINITIONS(Keyboard);

	Keyboard::Keyboard(Game& game, LPDIRECTINPUT8 input)
		: GameComponent(game), m_input {input}, m_device {} {
		assert(m_input != nullptr);
		ZeroMemory(m_curr_state, sizeof(m_curr_state));
		ZeroMemory(m_last_state, sizeof(m_last_state));
	}

	Keyboard::~Keyboard() {
		if (m_device != nullptr) {
			m_device->Unacquire();
			m_device->Release();
			m_device = nullptr;
		}
		ReleaseObject(m_input);
	}

	const byte* const Keyboard::curr_state() const { return m_curr_state; }
	const byte* const Keyboard::last_state() const { return m_last_state; }

	void Keyboard::init() {
		if (FAILED(m_input->CreateDevice(GUID_SysKeyboard, &m_device, nullptr))) {
			throw GameException("LPDIRECTINPUT8::CreateDevice() failed.");
		}
		if (FAILED(m_device->SetDataFormat(&c_dfDIKeyboard))) {
			throw GameException("LPDIRECTINPUTDEVICE8::SetDataFormat() failed.");
		}
		if (FAILED(m_device->SetCooperativeLevel(m_game->window_handle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
			throw GameException("LPDIRECTINPUTDEVICE8::SetCooperativeLevel() failed.");
		}
		if (FAILED(m_device->Acquire())) {
			throw GameException("LPDIRECTINPUTDEVICE8::Acquire() failed.");
		}
	}

	void Keyboard::update(const GameTime& game_time) {
		if (m_device != nullptr) {
			memcpy(m_last_state, m_curr_state, sizeof(m_curr_state));
			if (FAILED(m_device->GetDeviceState(sizeof(m_curr_state), LPVOID(m_curr_state)))) {
				if (SUCCEEDED(m_device->Acquire())) {
					m_device->GetDeviceState(sizeof(m_curr_state), LPVOID(m_curr_state));
				}
			}
		}
	}

	bool Keyboard::key_up(byte key) const {
		return ((m_curr_state[key] & 0x80) == 0);
	}

	bool Keyboard::key_down(byte key) const {
		return ((m_curr_state[key] & 0x80) != 0);
	}

	bool Keyboard::key_uped(byte key) const {
		return ((m_last_state[key] & 0x80) == 0);
	}

	bool Keyboard::key_downed(byte key) const {
		return ((m_last_state[key] & 0x80) != 0);
	}

	bool Keyboard::key_pressed(byte key) const {
		return (key_down(key) && key_uped(key));
	}

	bool Keyboard::key_released(byte key) const {
		return (key_up(key) && key_downed(key));
	}

	bool Keyboard::key_held(byte key) const {
		return (key_down(key) && key_downed(key));
	}

}