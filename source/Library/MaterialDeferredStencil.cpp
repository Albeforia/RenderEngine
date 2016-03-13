#include "MaterialDeferredStencil.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredStencil);

	MaterialDeferredStencil::MaterialDeferredStencil()
		: MATERIAL_VARIABLES_INITIALIZATION(WVP)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredStencil, WVP);

	void MaterialDeferredStencil::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(WVP);
	}

}