#pragma once

#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialDeferredPLight, POSITION, TEXCOORD)

	class MaterialDeferredPLight : public Material {

		RTTI_DECLARATIONS(MaterialDeferredPLight, Material);

		MATERIAL_VARIABLES_DECLARATION(AmbientColor, LightColor, LightPosition, LightAttenuation, CameraPosition, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	public:
		MaterialDeferredPLight();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}