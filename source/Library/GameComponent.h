#pragma once

#include "Common.h"

namespace Library {

	class Game;
	class GameTime;

	class GameComponent : public RTTI {

		RTTI_DECLARATIONS(GameComponent, RTTI);

	public:
		GameComponent();
		GameComponent(Game&);
		virtual ~GameComponent();

		GameComponent(const GameComponent&) = delete;
		GameComponent& operator=(const GameComponent&) = delete;

		Game* get_game() const;
		void set_game(Game&);
		bool& enabled();

		virtual void init();
		virtual void update(const GameTime&);

	protected:
		Game* m_game;
		bool m_enabled;

	};

}