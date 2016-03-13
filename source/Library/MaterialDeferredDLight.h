#pragma once

#include "Material.h"

namespace Library {

	class MaterialDeferredDLight : public Material {

		RTTI_DECLARATIONS(MaterialDeferredDLight, Material);

		MATERIAL_VARIABLES_DECLARATION(AmbientColor, LightColor, LightDirection,
									   CameraPosition, ScreenResolution,
									   PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	public:
		MaterialDeferredDLight();

		virtual void init(Effect*) override;

	};

}