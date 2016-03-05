#pragma once

#include <Windows.h>
#include <exception>

namespace Library {

	class GameTime;

	class GameClock {

	public:
		GameClock();
		GameClock(const GameClock&) = delete;
		GameClock& operator=(const GameClock&) = delete;

		const LARGE_INTEGER& start_time() const;
		const LARGE_INTEGER& current_time() const;
		const LARGE_INTEGER& last_time() const;

		void reset();
		double get_frequency() const;
		void get_time(LARGE_INTEGER&) const;
		void update_game_time(GameTime&);

	private:
		LARGE_INTEGER m_start_time;
		LARGE_INTEGER m_current_time;
		LARGE_INTEGER m_last_time;
		double m_frequency;

	};

}