#include "MaterialGaussianBlur.h"
#include "Game.h"
#include "VectorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialGaussianBlur);

	MaterialGaussianBlur::MaterialGaussianBlur(float blur_amount)
		: m_blur_amount {blur_amount},
		MATERIAL_VARIABLES_INITIALIZATION(ScreenResolution, SampleOffsets, SampleWeights, ColorBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialGaussianBlur, ScreenResolution, SampleOffsets, SampleWeights, ColorBuffer);

	void MaterialGaussianBlur::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(ScreenResolution, SampleOffsets, SampleWeights, ColorBuffer);
		int w = effect->game().screen_width();
		int h = effect->game().screen_height();
		init_sample_offsets(w, h);
		init_sample_weights();
		*m_ScreenResolution << XMVectorSet(w, h, 0, 0);
		*m_SampleOffsets << m_offsets;
		*m_SampleWeights << m_weights;
	}

	void MaterialGaussianBlur::init_sample_offsets(UINT width, UINT height) {
		float h_pixel_size = 1.0f / width;
		float v_pixel_size = 1.0f / height;
		UINT count = m_SampleOffsets->type_desc().Elements;
		m_offsets.resize(count);
		m_offsets[0] = Vector2Helper::Zero;
		for (UINT i = 0; i < count / 2; i++) {
			float offset = i * 2 + 1.5f;
			float h = h_pixel_size*offset;
			float v = v_pixel_size*offset;
			m_offsets[i * 2 + 1] = XMFLOAT2(h, v);
			m_offsets[i * 2 + 2] = XMFLOAT2(-h, -v);
		}
	}

	void MaterialGaussianBlur::init_sample_weights() {
		UINT count = m_SampleOffsets->type_desc().Elements;
		m_weights.resize(count);
		m_weights[0] = 1;
		float total = m_weights[0];
		for (UINT i = 0; i < count / 2; i++) {
			float m = static_cast<float>(i);
			float w = static_cast<float>(exp(-(m * m) / (2 * m_blur_amount * m_blur_amount)));
			m_weights[i * 2 + 1] = w;
			m_weights[i * 2 + 2] = w;
			total += w * 2;
		}
		// normalize
		for (UINT i = 0; i < count; i++) {
			m_weights[i] /= total;
		}
	}

}