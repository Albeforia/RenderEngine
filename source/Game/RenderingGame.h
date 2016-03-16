#pragma once

#include "Common.h"
#include "Game.h"
#include "dinput.h"

using namespace Library;

namespace Library {
	class Keyboard;
	class Mouse;
	class RenderStateHelper;
	class FullScreenRenderTarget;
	class Camera;
	class Geometry;
	class Material;
	class Effect;
	class Light;
	class Model;
}

namespace Rendering {

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

		RenderStateHelper* m_render_state_helper;
		// G-Buffers
		std::vector<FullScreenRenderTarget*> m_render_targets;
		ID3D11RenderTargetView** m_render_targets_raw;

		Camera* m_camera;

		Model* m_model;
		Geometry* m_sphere;
		Geometry* m_quad;

		Material* m_stencil_material;
		Effect* m_stencil_effect;
		void update_stencil_material();

		Material* m_light_material;
		Effect* m_light_effect;
		void update_light_material();

		Material* m_quad_material;
		Effect* m_quad_effect;
		void update_quad_material();

		Light* m_sun;
		Light* m_point_light;

		Material* m_test_material;
		Effect* m_test_effect;

		Material* m_blur_material;
		Effect* m_blur_effect;

	};

}