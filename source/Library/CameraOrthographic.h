#pragma once

#include "Camera.h"

namespace Library {

	class CameraOrthographic : public Camera {

		RTTI_DECLARATIONS(CameraOrthographic, Camera);

	public:
		CameraOrthographic(Game&);
		CameraOrthographic(Game&, float nearp, float farp,
						   float view_width, float view_height);
		virtual ~CameraOrthographic();

		virtual void update_projection() override;

		static const float DEFAULT_VIEW_WIDTH;
		static const float DEFAULT_VIEW_HEIGHT;

	protected:
		float m_view_width;
		float m_view_height;

	};

}