#pragma once

#include "GameComponent.h"

namespace Library {

	class Keyboard : public GameComponent {

		RTTI_DECLARATIONS(Keyboard, GameComponent);

	public:
		Keyboard(Game&, LPDIRECTINPUT8);
		~Keyboard();

		const byte* const curr_state() const;
		const byte* const last_state() const;

		virtual void init() override;
		virtual void update(const GameTime&) override;

		bool key_up(byte key) const;
		bool key_down(byte key) const;
		bool key_uped(byte key) const;
		bool key_downed(byte key) const;
		bool key_pressed(byte key) const;
		bool key_released(byte key) const;
		bool key_held(byte key) const;

	private:
		static const int KEY_COUNT = 256;

		LPDIRECTINPUT8 m_input;
		LPDIRECTINPUTDEVICE8 m_device;
		byte m_curr_state[KEY_COUNT];
		byte m_last_state[KEY_COUNT];

	};

}