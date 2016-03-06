#include "GameComponentDrawable.h"

namespace Library {

	RTTI_DEFINITIONS(GameComponentDrawable);

	GameComponentDrawable::GameComponentDrawable()
		: GameComponent(), m_visible {true}, m_camera {} {}

	GameComponentDrawable::GameComponentDrawable(Game& game)
		: GameComponent(game), m_visible {true}, m_camera {} {}

	GameComponentDrawable::GameComponentDrawable(Game& game, Camera& camera)
		: GameComponent(game), m_visible {true}, m_camera {&camera} {}

	GameComponentDrawable::~GameComponentDrawable() {}

	bool GameComponentDrawable::visible() const {
		return m_visible;
	}

	void GameComponentDrawable::set_visible(bool v) {
		m_visible = v;
	}

	void GameComponentDrawable::draw(const GameTime& game_time) {}

}