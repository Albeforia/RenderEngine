#include <memory>
#include "RenderingGame.h"
#include "GameException.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace Library;
using namespace Rendering;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE pre_instance, LPSTR cmd_line, int show_cmd) {
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	std::unique_ptr<Game> game(new RenderingGame(instance, L"RenderingClass", L"Real-Time 3D Rendering", show_cmd));
	try {
		game->run();
	}
	catch (const GameException& e) {
		MessageBox(game->window_handle(), e.whatw().c_str(), game->window_title().c_str(), MB_ABORTRETRYIGNORE);
	}
	return 0;
}