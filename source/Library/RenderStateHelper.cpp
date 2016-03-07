#include "RenderStateHelper.h"
#include "Game.h"

namespace Library {

	RenderStateHelper::RenderStateHelper(Game& game)
		: m_game {game}, m_rasterizer_state {}, m_blend_state {}, m_depth_stencil_state {},
		m_blend_factor {new FLOAT[4]}, m_sample_mask {UINT_MAX}, m_stencil_ref {UINT_MAX} {}

	RenderStateHelper::~RenderStateHelper() {
		ReleaseObject(m_rasterizer_state);
		ReleaseObject(m_blend_state);
		ReleaseObject(m_depth_stencil_state);
		DeleteObject(m_blend_factor);
	}

	void RenderStateHelper::reset_all(ID3D11DeviceContext* con) {
		con->RSSetState(nullptr);
		con->OMSetBlendState(nullptr, nullptr, UINT_MAX);
		con->OMSetDepthStencilState(nullptr, UINT_MAX);
	}

	void RenderStateHelper::save_rasterizer_state() {
		ReleaseObject(m_rasterizer_state);
		m_game.d3d_device_context()->RSGetState(&m_rasterizer_state);
	}
	void RenderStateHelper::save_blend_state() {
		ReleaseObject(m_blend_state);
		m_game.d3d_device_context()->OMGetBlendState(&m_blend_state, m_blend_factor, &m_sample_mask);
	}
	void RenderStateHelper::save_depth_stencil_state() {
		ReleaseObject(m_depth_stencil_state);
		m_game.d3d_device_context()->OMGetDepthStencilState(&m_depth_stencil_state, &m_stencil_ref);
	}
	void RenderStateHelper::save_all() {
		save_rasterizer_state();
		save_blend_state();
		save_depth_stencil_state();
	}
	void RenderStateHelper::restore_rasterizer_state() const {
		m_game.d3d_device_context()->RSSetState(m_rasterizer_state);
	}
	void RenderStateHelper::restore_blend_state() const {
		m_game.d3d_device_context()->OMSetBlendState(m_blend_state, m_blend_factor, m_sample_mask);
	}
	void RenderStateHelper::restore_depth_stencil_state() const {
		m_game.d3d_device_context()->OMSetDepthStencilState(m_depth_stencil_state, m_stencil_ref);
	}
	void RenderStateHelper::restore_all() const {
		restore_rasterizer_state();
		restore_blend_state();
		restore_depth_stencil_state();
	}

}