#pragma once

namespace Library {
	class GameTime {
	public:
		GameTime();

		double total_game_time() const;
		void set_total_game_time(double);

		double elapsed_game_time() const;
		void set_elapsed_game_time(double);

	private:
		double m_total_game_time;
		double m_elapsed_game_time;
	};
}