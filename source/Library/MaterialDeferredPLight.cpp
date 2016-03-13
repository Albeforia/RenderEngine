#include "MaterialDeferredPLight.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredPLight);

	MaterialDeferredPLight::MaterialDeferredPLight()
		: MATERIAL_VARIABLES_INITIALIZATION(ScreenResolution, VP, CameraPosition, World,
											LightColor, LightPosition, LightAttenuation,
											PositionBuffer, NormalBuffer, AlbedoSpecularBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredPLight,
								  ScreenResolution, VP, CameraPosition, World,
								  LightColor, LightPosition, LightAttenuation,
								  PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	void MaterialDeferredPLight::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(ScreenResolution, VP, CameraPosition, World,
									LightColor, LightPosition, LightAttenuation,
									PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);
	}

}