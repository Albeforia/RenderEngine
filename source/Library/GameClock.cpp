#include "GameClock.h"
#include "GameTime.h"

namespace Library {

	GameClock::GameClock()
		: m_start_time {}, m_current_time {}, m_last_time {}, m_frequency {} {
		m_frequency = get_frequency();
		reset();
	}

	const LARGE_INTEGER& GameClock::start_time() const {
		return m_start_time;
	}

	const LARGE_INTEGER& GameClock::current_time() const {
		return m_current_time;
	}

	const LARGE_INTEGER& GameClock::last_time() const {
		return m_last_time;
	}

	void GameClock::reset() {
		get_time(m_start_time);
		m_current_time = m_start_time;
		m_last_time = m_current_time;
	}

	double GameClock::get_frequency() const {
		LARGE_INTEGER f;
		if (!QueryPerformanceFrequency(&f)) {
			throw std::exception("QueryPerformanceFrequency() failed.");
		}
		return double(f.QuadPart);
	}

	void GameClock::get_time(LARGE_INTEGER& time) const {
		QueryPerformanceCounter(&time);
	}

	void GameClock::update_game_time(GameTime& game_time) {
		get_time(m_current_time);
		game_time.set_total_game_time((m_current_time.QuadPart - m_start_time.QuadPart) / m_frequency);
		game_time.set_elapsed_game_time((m_current_time.QuadPart - m_last_time.QuadPart) / m_frequency);
		m_last_time = m_current_time;
	}

}