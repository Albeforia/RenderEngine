#pragma once

#include "Material.h"

namespace Library {

	class MaterialGaussianBlur : public Material {

		RTTI_DECLARATIONS(MaterialGaussianBlur, Material);

		MATERIAL_VARIABLES_DECLARATION(ScreenResolution, SampleOffsets, SampleWeights, ColorBuffer);

	public:
		MaterialGaussianBlur(float blur_amount);

		virtual void init(Effect*) override;

	private:
		void init_sample_offsets(UINT width, UINT height);
		void init_sample_weights();

		std::vector<XMFLOAT2> m_offsets;
		std::vector<float> m_weights;
		float m_blur_amount;

	};

}