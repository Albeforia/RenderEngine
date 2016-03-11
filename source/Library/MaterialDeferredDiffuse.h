#pragma once

#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialDeferredDiffuse, POSITION, TEXCOORD)

	class MaterialDeferredDiffuse : public Material {

		RTTI_DECLARATIONS(MaterialDeferredDiffuse, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP, AmbientColor, LightColor, LightDirection, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	public:
		MaterialDeferredDiffuse();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}