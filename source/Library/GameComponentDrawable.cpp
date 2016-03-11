#include "GameComponentDrawable.h"
#include "MatrixHelper.h"

namespace Library {

	RTTI_DEFINITIONS(GameComponentDrawable);

	GameComponentDrawable::GameComponentDrawable()
		: GameComponent(), m_camera {},
		m_visible {true}, m_transform {MatrixHelper::Identity} {}

	GameComponentDrawable::GameComponentDrawable(Game& game)
		: GameComponent(game), m_camera {},
		m_visible {true}, m_transform {MatrixHelper::Identity} {}

	GameComponentDrawable::GameComponentDrawable(Game& game, Camera& camera)
		: GameComponent(game), m_camera {&camera},
		m_visible {true}, m_transform {MatrixHelper::Identity} {}

	GameComponentDrawable::~GameComponentDrawable() {}

	bool& GameComponentDrawable::visible() {
		return m_visible;
	}

	void GameComponentDrawable::translate(float x, float y, float z) {
		XMStoreFloat4x4(&m_transform,
						XMLoadFloat4x4(&m_transform) * XMMatrixTranslation(x, y, z));
	}

	void GameComponentDrawable::scale(float x, float y, float z) {
		XMStoreFloat4x4(&m_transform,
						XMLoadFloat4x4(&m_transform) * XMMatrixScaling(x, y, z));
	}

	void GameComponentDrawable::draw(const GameTime& game_time) {}

}