#include "Game.h"

namespace Library {

	const UINT Game::DEFAULT_SCREEN_WIDTH = 1280;
	const UINT Game::DEFAULT_SCREEN_HEIGHT = 720;

	Game::Game(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: m_instance {instance}, m_window_class {window_class}, m_window_title {window_title}, m_show_cmd {show_cmd},
		m_window_handle {}, m_window {}, m_game_clock {}, m_game_time {},
		m_screen_width {DEFAULT_SCREEN_WIDTH}, m_screen_height {DEFAULT_SCREEN_HEIGHT} {}

	Game::~Game() {}

	HINSTANCE Game::instance() const {
		return m_instance;
	}

	HWND Game::window_handle() const {
		return m_window_handle;
	}

	const WNDCLASSEX& Game::window() const {
		return m_window;
	}

	const std::wstring& Game::window_class() const {
		return m_window_class;
	}

	const std::wstring& Game::window_title() const {
		return m_window_title;
	}

	int Game::screen_width() const {
		return m_screen_width;
	}

	int Game::screen_height() const {
		return m_screen_height;
	}

	void Game::run() {
		init_window();
		init();
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		m_game_clock.reset();
		while (msg.message != WM_QUIT) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				m_game_clock.update_game_time(m_game_time);
				update(m_game_time);
				draw(m_game_time);
			}
		}
		shutdown();
	}

	void Game::exit() {
		PostQuitMessage(0);
	}

	void Game::shutdown() {
		UnregisterClass(m_window_class.c_str(), m_window.hInstance);
	}

	void Game::init() {}

	void Game::update(const GameTime& game_time) {}

	void Game::draw(const GameTime& game_time) {}

	void Game::init_window() {
		ZeroMemory(&m_window, sizeof(m_window));
		m_window.cbSize = sizeof(WNDCLASSEX);
		m_window.style = CS_CLASSDC;
		m_window.lpfnWndProc = WndProc;
		m_window.hInstance = m_instance;
		m_window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		m_window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		m_window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		m_window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		m_window.lpszClassName = m_window_class.c_str();

		RECT window_rectangle = {0, 0, m_screen_width, m_screen_height};
		AdjustWindowRect(&window_rectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&m_window);
		POINT center = center_window(m_screen_width, m_screen_height);
		m_window_handle = CreateWindow(
			m_window_class.c_str(), m_window_title.c_str(),
			WS_OVERLAPPEDWINDOW, center.x, center.y,
			window_rectangle.right - window_rectangle.left,
			window_rectangle.bottom - window_rectangle.top, nullptr, nullptr, m_instance, nullptr);

		ShowWindow(m_window_handle, m_show_cmd);
		UpdateWindow(m_window_handle);
	}

	LRESULT WINAPI Game::WndProc(HWND handle, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
		}
		return DefWindowProc(handle, msg, wp, lp);
	}

	POINT Game::center_window(int width, int height) {
		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);
		POINT center;
		center.x = (screen_width - width) / 2;
		center.y = (screen_height - height) / 2;
		return center;
	}

}