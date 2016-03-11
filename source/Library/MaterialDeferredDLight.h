#pragma once

#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialDeferredDLight, POSITION, TEXCOORD)

	class MaterialDeferredDLight : public Material {

		RTTI_DECLARATIONS(MaterialDeferredDLight, Material);

		MATERIAL_VARIABLES_DECLARATION(AmbientColor, LightColor, LightDirection, CameraPosition, SpecularColor, SpecularPower, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	public:
		MaterialDeferredDLight();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}