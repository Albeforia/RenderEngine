#pragma once

#include "Common.h"
#include "Game.h"

using namespace Library;

namespace Library {
	class Keyboard;
	class Mouse;
	class Camera;
	class RenderStateHelper;
	class InfoPanel;
}

namespace Rendering {

	class MaterialBasicDemo;

	class RenderingGame : public Game {

	public:
		RenderingGame(HINSTANCE, const std::wstring& window_class, const std::wstring& window_title, int show_cmd);
		~RenderingGame();

		virtual void init() override;
		virtual void shutdown() override;
		virtual void update(const GameTime&) override;
		virtual void draw(const GameTime&) override;

	private:
		static const XMVECTORF32 BACKGROUND_COLOR;

		LPDIRECTINPUT8 m_input;
		Keyboard* m_keyboard;
		Mouse* m_mouse;
		Camera* m_camera;

		RenderStateHelper* m_render_state_helper;

		InfoPanel* m_info_panel;
		MaterialBasicDemo* m_demo;

	};

}