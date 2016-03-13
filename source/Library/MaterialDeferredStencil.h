#pragma once

#include "Material.h"

namespace Library {

	class MaterialDeferredStencil : public Material {

		RTTI_DECLARATIONS(MaterialDeferredStencil, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP);

	public:
		MaterialDeferredStencil();

		virtual void init(Effect*) override;

	};

}