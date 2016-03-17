#pragma once

#include "Material.h"

namespace Library {

	class MaterialDownSampling : public Material {

		RTTI_DECLARATIONS(MaterialDownSampling, Material);

		MATERIAL_VARIABLES_DECLARATION(ScreenResolution, ColorBuffer);

	public:
		MaterialDownSampling(UINT down_sampling_ratio);

		virtual void init(Effect*) override;

	private:
		UINT m_down_sampling_ratio;

	};

}