#include "CameraPerspective.h"
#include "Game.h"

namespace Library {

	RTTI_DEFINITIONS(CameraPerspective);

	const float CameraPerspective::DEFAULT_FOV = XM_PIDIV4;
	const float CameraPerspective::DEFAULT_FOCUS_DISTANCE = 20.0f;
	const float CameraPerspective::DEFAULT_FOCUS_RANGE = 15.0f;

	CameraPerspective::CameraPerspective(Game& game)
		: Camera(game),
		m_fov {DEFAULT_FOV}, m_ratio {game.aspect_ratio()},
		m_focus_distance {DEFAULT_FOCUS_DISTANCE}, m_focus_range {DEFAULT_FOCUS_RANGE} {}

	CameraPerspective::CameraPerspective(Game& game, float fov, float ratio, float nearp, float farp,
										 float focus_distance, float focus_range)
		: Camera(game, nearp, farp),
		m_fov {fov}, m_ratio {ratio}, m_focus_distance {focus_distance}, m_focus_range {focus_range} {}

	CameraPerspective::~CameraPerspective() {}

	float CameraPerspective::focus_distance() const { return m_focus_distance; }
	float CameraPerspective::focus_range() const { return m_focus_range; }

	void CameraPerspective::update_projection() {
		XMMATRIX projection = XMMatrixPerspectiveFovRH(m_fov, m_ratio, m_nearp, m_farp);
		XMStoreFloat4x4(&m_projection, projection);
	}

}