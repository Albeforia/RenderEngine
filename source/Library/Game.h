#pragma once

#include "Common.h"
#include "GameClock.h"
#include "GameTime.h"
#include "GameComponent.h"
#include "ServiceContainer.h"

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
		float aspect_ratio() const;

		// FIXME add getter implementation
		ID3D11Device1* d3d_device() const;
		ID3D11DeviceContext1* d3d_device_context() const;
		bool depth_stencil_enabled() const;
		bool is_full_screen() const;
		//const D3D11_TEXTURE2D_DESC& buffer_desc() const;
		const D3D11_VIEWPORT& viewport() const;

		const std::vector<GameComponent*>& components() const;
		const ServiceContainer& services() const;

		virtual void run();
		virtual void exit();
		virtual void init();
		virtual void update(const GameTime&);
		virtual void draw(const GameTime&);

		void reset_render_targets();

	protected:
		virtual void init_window();
		virtual void shutdown();
		virtual void init_dx();

		static const UINT DEFAULT_SCREEN_WIDTH;
		static const UINT DEFAULT_SCREEN_HEIGHT;
		static const UINT DEFAULT_FRAME_RATE;
		static const UINT DEFAULT_MSAA_COUNT;

		HINSTANCE m_instance;
		HWND m_window_handle;
		WNDCLASSEX m_window;
		std::wstring m_window_class;
		std::wstring m_window_title;
		UINT m_screen_width;
		UINT m_screen_height;
		int m_show_cmd;

		D3D_FEATURE_LEVEL m_feature_level;
		ID3D11Device1* m_d3d_device;
		ID3D11DeviceContext1* m_d3d_device_context;
		IDXGISwapChain1* m_swap_chain;

		UINT m_frame_rate;
		bool m_is_full_screen;
		bool m_depth_stencil_enabled;
		bool m_msaa_enabled;
		UINT m_msaa_count;
		UINT m_msaa_quality_levels;

		//ID3D11Texture2D* m_dsbuffer;
		//D3D11_TEXTURE2D_DESC m_buffer_desc;
		ID3D11RenderTargetView* m_render_target_back;
		ID3D11DepthStencilView* m_depth_stencil_back;
		D3D11_VIEWPORT m_viewport;

		std::vector<GameComponent*> m_components;
		ServiceContainer m_services;

		GameClock m_game_clock;
		GameTime m_game_time;

	private:
		POINT center_window(int width, int height);

		static LRESULT WINAPI WndProc(HWND, UINT msg, WPARAM, LPARAM);

	};

}