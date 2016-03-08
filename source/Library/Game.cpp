#include "Game.h"
#include "GameException.h"
#include "GameComponentDrawable.h"

namespace Library {

	const UINT Game::DEFAULT_SCREEN_WIDTH = 1280;
	const UINT Game::DEFAULT_SCREEN_HEIGHT = 720;
	const UINT Game::DEFAULT_FRAME_RATE = 60;
	const UINT Game::DEFAULT_MSAA_COUNT = 4;

	Game::Game(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: m_instance {instance}, m_window_class {window_class}, m_window_title {window_title}, m_show_cmd {show_cmd},
		m_window_handle {}, m_window {},
		m_screen_width {DEFAULT_SCREEN_WIDTH}, m_screen_height {DEFAULT_SCREEN_HEIGHT},
		m_feature_level {D3D_FEATURE_LEVEL_9_1}, m_d3d_device {}, m_d3d_device_context {},
		m_swap_chain {}, m_frame_rate {DEFAULT_FRAME_RATE}, m_is_full_screen {},
		m_dsbuffer_enabled {}, m_msaa_enabled {}, m_msaa_count {DEFAULT_MSAA_COUNT}, m_msaa_quality_levels {},
		m_dsbuffer {}, m_render_target_view {}, m_depth_stencil_view {}, m_viewport {},
		m_game_clock {}, m_game_time {}, m_components {}, m_services {} {}

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

	float Game::aspect_ratio() const {
		return static_cast<float>(m_screen_width) / m_screen_height;
	}

	ID3D11Device1* Game::d3d_device() const {
		return m_d3d_device;
	}

	ID3D11DeviceContext1* Game::d3d_device_context() const {
		return m_d3d_device_context;
	}

	const std::vector<GameComponent*>& Game::components() const { return m_components; }
	const ServiceContainer& Game::services() const { return m_services; }

	void Game::run() {
		init_window();
		init_dx();
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
		ReleaseObject(m_render_target_view);
		ReleaseObject(m_depth_stencil_view);
		ReleaseObject(m_swap_chain);
		ReleaseObject(m_dsbuffer);
		if (m_d3d_device_context != nullptr) {
			m_d3d_device_context->ClearState();
		}
		ReleaseObject(m_d3d_device_context);
		ReleaseObject(m_d3d_device);
		UnregisterClass(m_window_class.c_str(), m_window.hInstance);
	}

	void Game::init() {
		for (GameComponent* c : m_components) {
			c->init();
		}
	}

	void Game::update(const GameTime& game_time) {
		for (GameComponent* c : m_components) {
			if (c->enabled()) {
				c->update(game_time);
			}
		}
	}

	void Game::draw(const GameTime& game_time) {
		for (GameComponent* c : m_components) {
			// TODO use seperate vectors for drawable and non-drawable
			auto* drawable = c->As<GameComponentDrawable>();
			if (drawable != nullptr && drawable->visible()) {
				drawable->draw(game_time);
			}
		}
	}

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

	void Game::init_dx() {
		HRESULT hr;
		UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		ID3D11Device* dev = nullptr;
		ID3D11DeviceContext* con = nullptr;
		if (FAILED(hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &dev, &m_feature_level, &con))) {
			throw GameException("D3D11CreateDevice() failed.", hr);
		}
		if (FAILED(hr = dev->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_d3d_device)))) {
			throw GameException("ID3D11Device::QueryInterface() failed.", hr);
		}
		if (FAILED(hr = con->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_d3d_device_context)))) {
			throw GameException("ID3D11DeviceContext1::QueryInterface() failed.", hr);
		}
		ReleaseObject(dev);
		ReleaseObject(con);

		m_d3d_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_msaa_count, &m_msaa_quality_levels);
		if (m_msaa_quality_levels == 0) {
			throw GameException("Unsupported MSAA quality.");
		}

		DXGI_SWAP_CHAIN_DESC1 chain_desc;
		ZeroMemory(&chain_desc, sizeof(chain_desc));
		chain_desc.Width = m_screen_width;
		chain_desc.Height = m_screen_height;
		chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (m_msaa_enabled) {
			chain_desc.SampleDesc.Count = m_msaa_count;
			chain_desc.SampleDesc.Quality = m_msaa_quality_levels - 1;
		}
		else {
			chain_desc.SampleDesc.Count = 1;
			chain_desc.SampleDesc.Quality = 0;
		}

		chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		chain_desc.BufferCount = 1;
		chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		IDXGIDevice* dxgi_dev = nullptr;
		if (FAILED(hr = m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_dev)))) {
			throw GameException("ID3D11Device::QueryInterface() failed.", hr);
		}
		IDXGIAdapter* dxgi_adapter = nullptr;
		if (FAILED(hr = dxgi_dev->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgi_adapter)))) {
			ReleaseObject(dxgi_dev);
			throw GameException("IDXGIDevice::GetParent() failed retriving adapter.", hr);
		}
		IDXGIFactory2* dxgi_factory = nullptr;
		if (FAILED(hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgi_factory)))) {
			ReleaseObject(dxgi_dev);
			ReleaseObject(dxgi_adapter);
			throw GameException("IDXGIAdapter::GetParent() failed retriving factory.", hr);
		}

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC full_screen_desc;
		ZeroMemory(&full_screen_desc, sizeof(full_screen_desc));
		full_screen_desc.RefreshRate.Numerator = m_frame_rate;
		full_screen_desc.RefreshRate.Denominator = 1;
		full_screen_desc.Windowed = !m_is_full_screen;

		if (FAILED(hr = dxgi_factory->CreateSwapChainForHwnd(dxgi_dev, m_window_handle, &chain_desc, &full_screen_desc, nullptr, &m_swap_chain))) {
			ReleaseObject(dxgi_dev);
			ReleaseObject(dxgi_adapter);
			ReleaseObject(dxgi_factory);
			throw GameException("IDXGIFactory2::CreateSwapChainForHwnd() failed.", hr);
		}

		ReleaseObject(dxgi_dev);
		ReleaseObject(dxgi_adapter);
		ReleaseObject(dxgi_factory);

		ID3D11Texture2D* buffer;
		if (FAILED(hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&buffer)))) {
			throw GameException("IDXGISwapChain1::GetBuffer() failed.", hr);
		}
		buffer->GetDesc(&m_buffer_desc);

		if (FAILED(hr = m_d3d_device->CreateRenderTargetView(buffer, nullptr, &m_render_target_view))) {
			ReleaseObject(buffer);
			throw GameException("ID3D11Device1::CreateRenderTargetView() failed.", hr);
		}
		ReleaseObject(buffer);

		if (m_dsbuffer_enabled) {
			D3D11_TEXTURE2D_DESC ds_desc;
			ZeroMemory(&ds_desc, sizeof(ds_desc));
			ds_desc.Width = m_screen_width;
			ds_desc.Height = m_screen_height;
			ds_desc.MipLevels = 1;
			ds_desc.ArraySize = 1;
			ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			ds_desc.Usage = D3D11_USAGE_DEFAULT;
			if (m_msaa_enabled) {
				ds_desc.SampleDesc.Count = m_msaa_count;
				ds_desc.SampleDesc.Quality = m_msaa_quality_levels - 1;
			}
			else {
				ds_desc.SampleDesc.Count = 1;
				ds_desc.SampleDesc.Quality = 0;
			}
			if (FAILED(hr = m_d3d_device->CreateTexture2D(&ds_desc, nullptr, &m_dsbuffer))) {
				throw GameException("ID3D11Device1::CreateTexture2D() failed.", hr);
			}
			if (FAILED(hr = m_d3d_device->CreateDepthStencilView(m_dsbuffer, nullptr, &m_depth_stencil_view))) {
				throw GameException("ID3D11Device1::CreateDepthStencilView() failed.", hr);
			}
		}

		m_d3d_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = static_cast<float>(m_screen_width);
		m_viewport.Height = static_cast<float>(m_screen_height);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_d3d_device_context->RSSetViewports(1, &m_viewport);
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