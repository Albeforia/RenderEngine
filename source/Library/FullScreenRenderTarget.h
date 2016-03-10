#pragma once

#include "Common.h"

namespace Library {

	class Game;

	class FullScreenRenderTarget {
	public:
		FullScreenRenderTarget(Game&);
		~FullScreenRenderTarget();

		ID3D11ShaderResourceView* output_texture() const;
		ID3D11RenderTargetView* render_target_view() const;
		ID3D11DepthStencilView* depth_stencil_view() const;

		void begin();
		void end();

	private:
		Game* m_game;
		ID3D11RenderTargetView* m_render_target_view;
		ID3D11DepthStencilView* m_depth_stencil_view;
		ID3D11ShaderResourceView* m_output_texture;
	};

}