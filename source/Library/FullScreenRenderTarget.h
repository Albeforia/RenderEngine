#pragma once

#include "Common.h"

namespace Library {

	class Game;

	class FullScreenRenderTarget {
	public:
		FullScreenRenderTarget(Game&,
							   ID3D11DepthStencilView* ds = nullptr, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		~FullScreenRenderTarget();

		ID3D11RenderTargetView* render_target() const;
		ID3D11DepthStencilView* depth_stencil() const;
		ID3D11ShaderResourceView* output_texture() const;

	private:
		Game* m_game;
		ID3D11RenderTargetView* m_render_target;
		ID3D11DepthStencilView* m_depth_stencil;
		ID3D11ShaderResourceView* m_output_texture;
	};

}