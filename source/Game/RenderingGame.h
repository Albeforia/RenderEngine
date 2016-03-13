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
	class FullScreenQuad;
	class Sphere;
	class Material;
	class Effect;
	class Light;
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
		// MRT for deferred shading
		std::vector<FullScreenRenderTarget*> m_render_targets;
		ID3D11RenderTargetView** m_render_targets_raw;

		Camera* m_camera;

		Sphere* m_sphere;
		Material* m_sphere_material;
		Effect* m_sphere_effect;
		void update_sphere_material();

		FullScreenQuad* m_quad;
		Material* m_quad_material;
		Effect* m_quad_effect;
		void update_quad_material();

		Light* m_sun;
		Light* m_point_light;

	};

}