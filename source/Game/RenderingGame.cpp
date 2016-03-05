#include "RenderingGame.h"
#include "GameException.h"

namespace Rendering {

	const XMVECTORF32 RenderingGame::BACKGROUND_COLOR = {0.392f, 0.584f, 0.929f, 1.0f};

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: Game(instance, window_class, window_title, show_cmd) {
		m_dsbuffer_enabled = true;
		m_msaa_enabled = true;
	}

	RenderingGame::~RenderingGame() {}

	void RenderingGame::init() {
		Game::init();
	}

	void RenderingGame::update(const GameTime& game_time) {
		Game::update(game_time);
	}

	void RenderingGame::draw(const GameTime& game_time) {
		m_d3d_device_context->ClearRenderTargetView(m_render_target_view, reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		Game::draw(game_time);
		HRESULT hr = m_swap_chain->Present(0, 0);
		if (FAILED(hr)) {
			throw GameException("IDXGISwapChain1::Present() failed.", hr);
		}
	}

}