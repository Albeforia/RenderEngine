#include "FullScreenRenderTarget.h"
#include "Game.h"
#include "GameException.h"

namespace Library {

	FullScreenRenderTarget::FullScreenRenderTarget(Game& game)
		: m_game {&game}, m_render_target_view {}, m_depth_stencil_view {}, m_output_texture {} {
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = game.screen_width();
		desc.Height = game.screen_height();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr;
		ID3D11Texture2D* tex = nullptr;
		ID3D11Device* dev = game.d3d_device();
		if (FAILED(hr = dev->CreateTexture2D(&desc, nullptr, &tex))) {
			throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
		}
		if (FAILED(hr = dev->CreateShaderResourceView(tex, nullptr, &m_output_texture))) {
			ReleaseObject(tex);
			throw GameException("IDXGIDevice::CreateShaderResourceView() failed.", hr);
		}
		if (FAILED(hr = dev->CreateRenderTargetView(tex, nullptr, &m_render_target_view))) {
			ReleaseObject(tex);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}
		ReleaseObject(tex);

		ZeroMemory(&desc, sizeof(desc));
		desc.Width = game.screen_width();
		desc.Height = game.screen_height();
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* tex1 = nullptr;
		if (FAILED(hr = dev->CreateTexture2D(&desc, nullptr, &tex1))) {
			throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
		}
		if (FAILED(hr = dev->CreateDepthStencilView(tex1, nullptr, &m_depth_stencil_view))) {
			ReleaseObject(tex1);
			throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", hr);
		}
		ReleaseObject(tex1);
	}

	FullScreenRenderTarget::~FullScreenRenderTarget() {
		ReleaseObject(m_output_texture);
		ReleaseObject(m_depth_stencil_view);
		ReleaseObject(m_render_target_view);
	}

	ID3D11ShaderResourceView* FullScreenRenderTarget::output_texture() const {
		return m_output_texture;
	}

	ID3D11RenderTargetView* FullScreenRenderTarget::render_target_view() const {
		return m_render_target_view;
	}

	ID3D11DepthStencilView* FullScreenRenderTarget::depth_stencil_view() const {
		return m_depth_stencil_view;
	}

	void FullScreenRenderTarget::begin() {
		m_game->d3d_device_context()->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
	}

	void FullScreenRenderTarget::end() {
		m_game->reset_render_targets();
	}

}