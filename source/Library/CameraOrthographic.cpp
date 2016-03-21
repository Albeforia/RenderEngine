#include "CameraOrthographic.h"

namespace Library {

	RTTI_DEFINITIONS(CameraOrthographic);

	const float CameraOrthographic::DEFAULT_VIEW_WIDTH = 20.0f;
	const float CameraOrthographic::DEFAULT_VIEW_HEIGHT = 20.0f;

	CameraOrthographic::CameraOrthographic(Game& game)
		: Camera(game),
		m_view_width {DEFAULT_VIEW_WIDTH}, m_view_height {DEFAULT_VIEW_HEIGHT} {}

	CameraOrthographic::CameraOrthographic(Game& game, float nearp, float farp,
										   float view_width, float view_height)
		: Camera(game, nearp, farp),
		m_view_width {view_width}, m_view_height {view_height} {}

	CameraOrthographic::~CameraOrthographic() {}

	void CameraOrthographic::update_projection() {
		XMMATRIX projection = XMMatrixOrthographicRH(m_view_width, m_view_height, m_nearp, m_farp);
		XMStoreFloat4x4(&m_projection, projection);
	}

}