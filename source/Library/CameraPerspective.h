#pragma once

#include "Camera.h"

namespace Library {

	class CameraPerspective : public Camera {

		RTTI_DECLARATIONS(CameraPerspective, Camera);

	public:
		CameraPerspective(Game&);
		CameraPerspective(Game&, float fov, float ratio,
						  float nearp, float farp,
						  float focus_distance, float focus_range);
		virtual ~CameraPerspective();

		float ratio() const;
		float fov() const;
		float focus_distance() const;
		float focus_range() const;

		virtual void update_projection() override;

		static const float DEFAULT_FOV;
		static const float DEFAULT_FOCUS_DISTANCE;
		static const float DEFAULT_FOCUS_RANGE;

	protected:
		float m_fov;
		float m_ratio;
		float m_focus_distance;
		float m_focus_range;

	};

}