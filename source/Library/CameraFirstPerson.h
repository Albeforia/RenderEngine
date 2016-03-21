#pragma once

#include "CameraPerspective.h"

namespace Library {

	class Keyboard;
	class Mouse;

	class CameraFirstPerson : public CameraPerspective {

		RTTI_DECLARATIONS(CameraFirstPerson, CameraPerspective);

	public:
		CameraFirstPerson(Game&);
		CameraFirstPerson(Game&, float fov, float ratio,
						  float nearp, float farp,
						  float focus_distance, float focus_range);
		virtual ~CameraFirstPerson();

		const Keyboard& keyboard() const;
		void set_keyboard(Keyboard&);

		const Mouse& mouse() const;
		void set_mouse(Mouse&);

		float& mouse_sensitivity();
		float& rotation_rate();
		float& movement_rate();

		virtual void init() override;
		virtual void update(const GameTime&) override;

		static const float DEFAULT_MOUSE_SENSITIVITY;
		static const float DEFAULT_ROTATION_RATE;
		static const float DEFAULT_MOVEMENT_RATE;

	protected:
		float m_mouse_sensitivity;
		float m_rotation_rate;
		float m_movement_rate;

		Keyboard* m_keyboard;
		Mouse* m_mouse;

	};

}