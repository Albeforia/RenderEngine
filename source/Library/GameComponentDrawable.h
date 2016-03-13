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

		bool& visible();
		void translate(float x, float y, float z);
		void scale(float x, float y, float z);

		virtual void draw(const GameTime&);

	protected:
		Camera* m_camera;

		bool m_visible;
		XMFLOAT4X4 m_transform;

	};

}