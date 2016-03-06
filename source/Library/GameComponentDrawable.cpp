#include "GameComponentDrawable.h"

namespace Library {

	RTTI_DEFINITIONS(GameComponentDrawable);

	GameComponentDrawable::GameComponentDrawable()
		: m_visible {true}, m_camera {} {
		GameComponent::GameComponent();
	}

	GameComponentDrawable::GameComponentDrawable(Game& game)
		: m_visible {true}, m_camera {} {
		GameComponent::GameComponent(game);
	}

	GameComponentDrawable::GameComponentDrawable(Game& game, Camera& camera)
		: m_visible {true}, m_camera {&camera} {
		GameComponent::GameComponent(game);
	}

	GameComponentDrawable::~GameComponentDrawable() {}

	bool GameComponentDrawable::visible() const {
		return m_visible;
	}

	void GameComponentDrawable::set_visible(bool v) {
		m_visible = v;
	}

	void GameComponentDrawable::draw(const GameTime& game_time) {}

}