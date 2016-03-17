#include "MaterialDownSampling.h"
#include "Effect.h"
#include "Game.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDownSampling);

	MaterialDownSampling::MaterialDownSampling(UINT down_sampling_ratio)
		: m_down_sampling_ratio {down_sampling_ratio},
		MATERIAL_VARIABLES_INITIALIZATION(ScreenResolution, ColorBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDownSampling, ScreenResolution, ColorBuffer);

	void MaterialDownSampling::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(ScreenResolution, ColorBuffer);
		int w = effect->game().screen_width();
		int h = effect->game().screen_height();
		*m_ScreenResolution << XMVectorSet(w / m_down_sampling_ratio, h / m_down_sampling_ratio, 0, 0);
	}

}