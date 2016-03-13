#include "MaterialDeferredDLight.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredDLight);

	MaterialDeferredDLight::MaterialDeferredDLight()
		: MATERIAL_VARIABLES_INITIALIZATION(AmbientColor, LightColor, LightDirection,
											CameraPosition, ScreenResolution,
											PositionBuffer, NormalBuffer, AlbedoSpecularBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredDLight, AmbientColor, LightColor, LightDirection,
								  CameraPosition, ScreenResolution,
								  PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	void MaterialDeferredDLight::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(AmbientColor, LightColor, LightDirection,
									CameraPosition, ScreenResolution,
									PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);
	}

}