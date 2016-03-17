#include "MaterialDoF.h"
#include "Camera.h"
#include "Effect.h"
#include "Game.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDoF);

	MaterialDoF::MaterialDoF(Camera* camera)
		: m_camera {*camera},
		MATERIAL_VARIABLES_INITIALIZATION(DoFParams, ScreenResolution, ColorBuffer, BlurBuffer, DepthBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDoF, DoFParams, ScreenResolution, ColorBuffer, BlurBuffer, DepthBuffer);

	void MaterialDoF::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(DoFParams, ScreenResolution, ColorBuffer, BlurBuffer, DepthBuffer);
		int w = effect->game().screen_width();
		int h = effect->game().screen_height();
		*m_ScreenResolution << XMVectorSet(w, h, 0, 0);
		float n = m_camera.nearp();
		float f = m_camera.farp();
		*m_DoFParams << XMVectorSet(m_camera.focus_distance(), m_camera.focus_range(),
									m_camera.nearp(), f / (n - f));
	}

}