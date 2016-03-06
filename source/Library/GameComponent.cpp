#include "GameComponent.h"
#include "GameTime.h"

namespace Library {

	RTTI_DEFINITIONS(GameComponent);

	GameComponent::GameComponent()
		: m_game {}, m_enabled {true} {}

	GameComponent::GameComponent(Game& game)
		: GameComponent() {
		m_game = &game;
	}

	GameComponent::~GameComponent() {}

	Game* GameComponent::get_game() const {
		return m_game;
	}

	void GameComponent::set_game(Game& game) {
		m_game = &game;
	}

	bool GameComponent::enabled() const {
		return m_enabled;
	}

	void GameComponent::set_enabled(bool e) {
		m_enabled = e;
	}

	void GameComponent::init() {}

	void GameComponent::update(const GameTime& game_time) {}

}