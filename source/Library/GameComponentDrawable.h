#pragma once

#include "GameComponent.h"

namespace Library {

	class Camera;

	class GameComponentDrawable : public GameComponent {

		RTTI_DECLARATIONS(GameComponentDrawable, GameComponent);

	public:
		GameComponentDrawable();
		GameComponentDrawable(Game&);
		GameComponentDrawable(Game&, Camera&);
		virtual ~GameComponentDrawable();

		GameComponentDrawable(const GameComponentDrawable&) = delete;
		GameComponentDrawable& operator=(const GameComponentDrawable&) = delete;

		bool visible() const;
		void set_visible(bool);

		Camera* get_camera();
		void set_camera(Camera&);

		virtual void draw(const GameTime&);

	protected:
		bool m_visible;
		Camera* m_camera;

	};

}