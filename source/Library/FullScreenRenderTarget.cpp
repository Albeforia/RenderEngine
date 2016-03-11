#include "FullScreenRenderTarget.h"
#include "Game.h"
#include "GameException.h"

namespace Library {

	FullScreenRenderTarget::FullScreenRenderTarget(Game& game, bool enable_depth_stencil)
		: m_game {&game}, m_render_target {}, m_depth_stencil {}, m_output_texture {},
		m_depth_stencil_enabled {enable_depth_stencil} {
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
		if (FAILED(hr = dev->CreateRenderTargetView(tex, nullptr, &m_render_target))) {
			ReleaseObject(tex);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}
		ReleaseObject(tex);

		if (enable_depth_stencil) {
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
			if (FAILED(hr = dev->CreateDepthStencilView(tex1, nullptr, &m_depth_stencil))) {
				ReleaseObject(tex1);
				throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", hr);
			}
			ReleaseObject(tex1);
		}
	}

	FullScreenRenderTarget::~FullScreenRenderTarget() {
		ReleaseObject(m_output_texture);
		if (m_depth_stencil_enabled) {
			ReleaseObject(m_depth_stencil);
		}
		ReleaseObject(m_render_target);
	}

	ID3D11ShaderResourceView* FullScreenRenderTarget::output_texture() const {
		return m_output_texture;
	}

	ID3D11RenderTargetView* FullScreenRenderTarget::render_target() const {
		return m_render_target;
	}

	ID3D11DepthStencilView* FullScreenRenderTarget::depth_stencil() const {
		return m_depth_stencil;
	}

	void FullScreenRenderTarget::set_depth_stencil(ID3D11DepthStencilView* pds) {
		m_depth_stencil = pds;
	}

}