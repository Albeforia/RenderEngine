#include "GameComponent.h"
#include "GameTime.h"

namespace Library {

	RTTI_DEFINITIONS(GameComponent);

	GameComponent::GameComponent()
		: m_game {}, m_enabled {true} {}

	GameComponent::GameComponent(Game& game)
		: m_game {&game}, m_enabled {true} {}

	GameComponent::~GameComponent() {}

	Game* GameComponent::get_game() const {
		return m_game;
	}

	void GameComponent::set_game(Game& game) {
		m_game = &game;
	}

	bool& GameComponent::enabled() {
		return m_enabled;
	}

	void GameComponent::init() {}

	void GameComponent::update(const GameTime& game_time) {}

}