#pragma once

#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialDiffuse, POSITION, TEXCOORD, NORMAL)

	class MaterialDiffuse : public Material {

		RTTI_DECLARATIONS(MaterialDiffuse, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP, World, AmbientColor, LightColor, LightDirection, ColorTexture);

	public:
		MaterialDiffuse();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}