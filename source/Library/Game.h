#pragma once

#include <Windows.h>
#include <string>
#include "GameClock.h"
#include "GameTime.h"

namespace Library {

	class Game {

	public:
		Game(HINSTANCE, const std::wstring& window_class, const std::wstring& window_title, int show_cmd);
		virtual ~Game();

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		HINSTANCE instance() const;
		HWND window_handle() const;
		const WNDCLASSEX& window() const;
		const std::wstring& window_class() const;
		const std::wstring& window_title() const;
		int screen_width() const;
		int screen_height() const;

		virtual void run();
		virtual void exit();
		virtual void init();
		virtual void update(const GameTime&);
		virtual void draw(const GameTime&);

	protected:
		virtual void init_window();
		virtual void shutdown();

		static const UINT DEFAULT_SCREEN_WIDTH;
		static const UINT DEFAULT_SCREEN_HEIGHT;

		HINSTANCE m_instance;
		HWND m_window_handle;
		WNDCLASSEX m_window;
		std::wstring m_window_class;
		std::wstring m_window_title;
		UINT m_screen_width;
		UINT m_screen_height;
		int m_show_cmd;

		GameClock m_game_clock;
		GameTime m_game_time;

	private:
		POINT center_window(int width, int height);

		static LRESULT WINAPI WndProc(HWND, UINT msg, WPARAM, LPARAM);

	};

}