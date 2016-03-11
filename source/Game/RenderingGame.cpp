#include "RenderingGame.h"
#include "GameException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "RenderStateHelper.h"
#include "FullScreenRenderTarget.h"
#include "CameraFirstPerson.h"
#include "ColorHelper.h"
#include "EarthDemo.h"
#include "Utility.h"
#include "Effect.h"
#include "MaterialBasic.h"
#include "FullScreenQuad.h"
#include "MatrixHelper.h"

namespace Rendering {

	const XMVECTORF32 RenderingGame::BACKGROUND_COLOR = ColorHelper::CornflowerBlue;

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: Game(instance, window_class, window_title, show_cmd),
		m_render_state_helper {}, m_render_targets {} {
		m_depth_stencil_enabled = false;	// no need for deferred shading
		m_msaa_enabled = true;
	}

	RenderingGame::~RenderingGame() {}

	void RenderingGame::init() {
		// input
		if (FAILED(DirectInput8Create(m_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_input, nullptr))) {
			throw GameException("DirectInput8Create() failed.");
		}

		m_keyboard = new Keyboard(*this, m_input);
		m_components.push_back(m_keyboard);
		m_services.add_service(Keyboard::TypeIdClass(), m_keyboard);

		m_mouse = new Mouse(*this, m_input);
		m_components.push_back(m_mouse);
		m_services.add_service(Mouse::TypeIdClass(), m_mouse);

		// render
		m_render_state_helper = new RenderStateHelper(*this);

		auto* position = new FullScreenRenderTarget(*this, true);
		auto* normal = new FullScreenRenderTarget(*this, false);
		auto* albedo_specular = new FullScreenRenderTarget(*this, false);
		//normal->set_depth_stencil(position->depth_stencil());
		//albedo_specular->set_depth_stencil(position->depth_stencil());
		m_render_targets.push_back(position);
		m_render_targets.push_back(normal);
		m_render_targets.push_back(albedo_specular);
		m_render_targets_raw = new ID3D11RenderTargetView*[3] {
			position->render_target(),
			normal->render_target(),
			albedo_specular->render_target()
		};

		// camera
		m_camera = new CameraFirstPerson(*this);
		m_components.push_back(m_camera);

		// other
		m_components.push_back(new EarthDemo(*this, *m_camera, L"content\\textures\\Earth.dds"));

		// only for test MRT
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		m_quad_effect = new Effect(*this);
		m_quad_effect->load(L"content\\effects\\basic.cso");
		m_quad_material = new MaterialBasic();
		m_quad_material->init(m_quad_effect);
		m_quad = new FullScreenQuad(*this, *m_quad_material);
		m_quad->init();
		m_quad->set_active_technique("main11", "p0");
		m_quad->set_update_material(std::bind(&RenderingGame::update_quad_material, this));

		// init each component
		Game::init();

		//
		m_camera->set_position(0, 0, 25.0f);
	}

	void RenderingGame::shutdown() {
		for (auto* t : m_render_targets) {
			DeleteObject(t);
		}
		DeleteObject(m_render_targets_raw);
		DeleteObject(m_render_state_helper);
		ReleaseObject(m_input);
		DeleteObject(m_quad_effect);
		DeleteObject(m_quad_material);
		DeleteObject(m_quad);
		// keyboard, mouse and camera will be deleted with components
		Game::shutdown();
	}

	void RenderingGame::update(const GameTime& game_time) {
		Game::update(game_time);
	}

	void RenderingGame::draw(const GameTime& game_time) {
		// geometry pass
		m_d3d_device_context->OMSetRenderTargets(3, m_render_targets_raw, m_render_targets[0]->depth_stencil());
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[0], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[1], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[2], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearDepthStencilView(m_render_targets[0]->depth_stencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		Game::draw(game_time);

		/*
		// light pass
		reset_render_targets();
		m_d3d_device_context->ClearRenderTargetView(m_render_target_back, reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearDepthStencilView(m_depth_stencil_back, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// switch effect
		Game::draw(game_time);
		*/

		// only for test MRT
		reset_render_targets();
		m_d3d_device_context->ClearRenderTargetView(m_render_target_back, reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		//m_d3d_device_context->ClearDepthStencilView(m_depth_stencil_back, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_quad->draw(game_time);

		// swap
		HRESULT hr = m_swap_chain->Present(0, 0);
		if (FAILED(hr)) {
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}

	void RenderingGame::update_quad_material() {
		m_quad_material->WVP() << XMLoadFloat4x4(&MatrixHelper::Identity);
		m_quad_material->DiffuseTexture() << m_render_targets[2]->output_texture();
	}

}