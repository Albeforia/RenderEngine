#include "RenderingGame.h"
#include "GameException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "CameraFirstPerson.h"
#include "RenderStateHelper.h"
#include "MaterialBasicDemo.h"
#include "Skybox.h"
#include "InfoPanel.h"

namespace Rendering {

	const XMVECTORF32 RenderingGame::BACKGROUND_COLOR = {0.392f, 0.584f, 0.929f, 1.0f};

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: Game(instance, window_class, window_title, show_cmd) {
		m_dsbuffer_enabled = true;
		m_msaa_enabled = true;
	}

	RenderingGame::~RenderingGame() {}

	void RenderingGame::init() {
		if (FAILED(DirectInput8Create(m_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_input, nullptr))) {
			throw GameException("DirectInput8Create() failed.");
		}

		m_keyboard = new Keyboard(*this, m_input);
		m_components.push_back(m_keyboard);
		m_services.add_service(Keyboard::TypeIdClass(), m_keyboard);

		m_mouse = new Mouse(*this, m_input);
		m_components.push_back(m_mouse);
		m_services.add_service(Mouse::TypeIdClass(), m_mouse);

		m_camera = new CameraFirstPerson(*this);
		m_components.push_back(m_camera);
		m_services.add_service(CameraFirstPerson::TypeIdClass(), m_camera);

		//m_info_panel = new InfoPanel(*this);
		//m_components.push_back(m_info_panel);

		/*m_demo = new MaterialBasicDemo(*this, *m_camera);
		m_components.push_back(m_demo);*/

		m_components.push_back(new Skybox(*this, *m_camera, L"content\\textures\\Lycksele2.dds", 4.0f));

		m_render_state_helper = new RenderStateHelper(*this);

		Game::init();

		m_camera->set_position(0, 0, 0);
	}

	void RenderingGame::shutdown() {
		for (auto* c : m_components) {
			DeleteObject(c);
		}
		DeleteObject(m_render_state_helper);
		ReleaseObject(m_input);
		Game::shutdown();
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