#pragma once

#include "Common.h"
#include "Material.h"

namespace Library {

	class MaterialSkybox : public Material {

		RTTI_DECLARATIONS(MaterialSkybox, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP, SkyboxTexture);

	public:
		MaterialSkybox();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}