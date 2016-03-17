#pragma once

#include "Common.h"

namespace Library {

	class Game;

	class FullScreenRenderTarget {
	public:
		FullScreenRenderTarget(Game&, bool enable_depth_stencil, DXGI_FORMAT);
		~FullScreenRenderTarget();

		ID3D11RenderTargetView* render_target() const;
		ID3D11DepthStencilView* depth_stencil() const;
		void set_depth_stencil(ID3D11DepthStencilView*);
		ID3D11ShaderResourceView* output_texture() const;
		ID3D11ShaderResourceView* output_depth_texture() const;

	private:
		Game* m_game;
		ID3D11RenderTargetView* m_render_target;
		ID3D11DepthStencilView* m_depth_stencil;
		ID3D11ShaderResourceView* m_texture;
		ID3D11ShaderResourceView* m_depth_texture;
		bool m_depth_stencil_enabled;
	};

}