#pragma once

#include "Material.h"

namespace Library {

	class Camera;

	class MaterialDoF : public Material {

		RTTI_DECLARATIONS(MaterialDoF, Material);

		MATERIAL_VARIABLES_DECLARATION(DoFParams, ScreenResolution, ColorBuffer, BlurBuffer, DepthBuffer);

	public:
		MaterialDoF(Camera*);

		virtual void init(Effect*) override;

	private:
		const Camera& m_camera;

	};

}