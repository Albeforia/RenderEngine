#include "FullScreenRenderTarget.h"
#include "Game.h"
#include "GameException.h"

namespace Library {

	FullScreenRenderTarget::FullScreenRenderTarget(Game& game, bool enable_depth_stencil,
												   DXGI_FORMAT format, UINT down_sampling_ratio)
		: m_game {&game}, m_render_target {}, m_depth_stencil {}, m_depth_stencil_enabled {enable_depth_stencil},
		m_texture {}, m_depth_texture {} {
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = game.screen_width() / down_sampling_ratio;
		desc.Height = game.screen_height() / down_sampling_ratio;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr;
		ID3D11Texture2D* buffer1 = nullptr;
		ID3D11Device* dev = game.d3d_device();
		if (FAILED(hr = dev->CreateTexture2D(&desc, nullptr, &buffer1))) {
			throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
		}
		if (FAILED(hr = dev->CreateShaderResourceView(buffer1, nullptr, &m_texture))) {
			ReleaseObject(buffer1);
			throw GameException("IDXGIDevice::CreateShaderResourceView() failed.", hr);
		}
		if (FAILED(hr = dev->CreateRenderTargetView(buffer1, nullptr, &m_render_target))) {
			ReleaseObject(buffer1);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}
		ReleaseObject(buffer1);

		if (enable_depth_stencil) {
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = game.screen_width() / down_sampling_ratio;
			desc.Height = game.screen_height() / down_sampling_ratio;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			desc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
			ZeroMemory(&dsv_desc, sizeof(dsv_desc));
			dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
			ZeroMemory(&srv_desc, sizeof(srv_desc));
			srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srv_desc.Texture2D.MipLevels = -1;

			ID3D11Texture2D* buffer2 = nullptr;
			if (FAILED(hr = dev->CreateTexture2D(&desc, nullptr, &buffer2))) {
				throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
			}
			if (FAILED(hr = dev->CreateShaderResourceView(buffer2, &srv_desc, &m_depth_texture))) {
				ReleaseObject(buffer2);
				throw GameException("IDXGIDevice::CreateShaderResourceView() failed.", hr);
			}
			if (FAILED(hr = dev->CreateDepthStencilView(buffer2, &dsv_desc, &m_depth_stencil))) {
				ReleaseObject(buffer2);
				throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", hr);
			}
			ReleaseObject(buffer2)
		}
	}

	FullScreenRenderTarget::~FullScreenRenderTarget() {
		ReleaseObject(m_texture);
		ReleaseObject(m_render_target);
		if (m_depth_stencil_enabled) {
			ReleaseObject(m_depth_texture);
			ReleaseObject(m_depth_stencil);
		}
	}

	ID3D11ShaderResourceView* FullScreenRenderTarget::output_texture() const {
		return m_texture;
	}

	ID3D11ShaderResourceView* FullScreenRenderTarget::output_depth_texture() const {
		return m_depth_texture;
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