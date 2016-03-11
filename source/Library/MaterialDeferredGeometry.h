#pragma once

#include "Common.h"
#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialDeferredGeometry, POSITION, NORMAL, TEXCOORD)

	class MaterialDeferredGeometry : public Material {

		RTTI_DECLARATIONS(MaterialDeferredGeometry, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP, World, SpecularPower, DiffuseTexture, SpecularTexture)

	public:
		MaterialDeferredGeometry();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}