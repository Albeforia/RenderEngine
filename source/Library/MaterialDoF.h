#pragma once

#include "Material.h"

namespace Library {

	class CameraPerspective;

	class MaterialDoF : public Material {

		RTTI_DECLARATIONS(MaterialDoF, Material);

		MATERIAL_VARIABLES_DECLARATION(DoFParams, ScreenResolution, ColorBuffer, BlurBuffer, DepthBuffer);

	public:
		MaterialDoF(CameraPerspective*);

		virtual void init(Effect*) override;

	private:
		const CameraPerspective& m_camera;

	};

}