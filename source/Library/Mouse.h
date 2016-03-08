#pragma once

#include "GameComponent.h"

namespace Library {

	class GameTime;

	enum MouseButtons {
		MouseButtonLeft = 0,
		MouseButtonRight,
		MouseButtonMiddle,
		MouseButtonX1
	};

	class Mouse : public GameComponent {

		RTTI_DECLARATIONS(Mouse, GameComponent);

	public:
		Mouse(Game&, LPDIRECTINPUT8);
		~Mouse();

		LPDIMOUSESTATE curr_state();
		LPDIMOUSESTATE last_state();

		virtual void init() override;
		virtual void update(const GameTime&);

		long x() const;
		long y() const;
		long wheel() const;

		bool button_up(MouseButtons) const;
		bool button_down(MouseButtons) const;
		bool button_uped(MouseButtons) const;
		bool button_downed(MouseButtons) const;
		bool button_pressed(MouseButtons) const;
		bool button_released(MouseButtons) const;
		bool button_held(MouseButtons) const;

	private:
		LPDIRECTINPUT8 m_input;
		LPDIRECTINPUTDEVICE8 m_device;
		DIMOUSESTATE m_curr_state;
		DIMOUSESTATE m_last_state;

		long m_x;
		long m_y;
		long m_wheel;

	};

}