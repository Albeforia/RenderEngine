#pragma once

#include "Common.h"
#include "Game.h"

using namespace Library;

namespace Rendering {

	class RenderingGame : public Game {

	public:
		RenderingGame(HINSTANCE, const std::wstring& window_class, const std::wstring& window_title, int show_cmd);
		~RenderingGame();

		virtual void init() override;
		virtual void update(const GameTime&) override;
		virtual void draw(const GameTime&) override;

	private:
		static const XMVECTORF32 BACKGROUND_COLOR;

	};

}