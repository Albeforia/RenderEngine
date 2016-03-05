#include "GameTime.h"

namespace Library {
	GameTime::GameTime()
		: m_total_game_time {}, m_elapsed_game_time {} {}

	double GameTime::total_game_time() const {
		return m_total_game_time;
	}

	void GameTime::set_total_game_time(double t) {
		m_total_game_time = t;
	}

	double GameTime::elapsed_game_time() const {
		return m_elapsed_game_time;
	}

	void GameTime::set_elapsed_game_time(double t) {
		m_elapsed_game_time = t;
	}
}