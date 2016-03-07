#pragma once

#include "Common.h"

namespace Library {

	class Game;

	class RenderStateHelper {

	public:
		RenderStateHelper(Game&);
		~RenderStateHelper();

		static void reset_all(ID3D11DeviceContext*);

		ID3D11RasterizerState* rasterizer_state();
		ID3D11BlendState* blend_state();
		ID3D11DepthStencilState* depth_stencil_state();

		void save_rasterizer_state();
		void save_blend_state();
		void save_depth_stencil_state();
		void save_all();
		void restore_rasterizer_state() const;
		void restore_blend_state() const;
		void restore_depth_stencil_state() const;
		void restore_all() const;

	private:
		Game& m_game;

		ID3D11RasterizerState* m_rasterizer_state;
		ID3D11BlendState* m_blend_state;
		ID3D11DepthStencilState* m_depth_stencil_state;
		FLOAT* m_blend_factor;
		UINT m_sample_mask;
		UINT m_stencil_ref;

	};

}