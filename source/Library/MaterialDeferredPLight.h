#pragma once

#include "Material.h"

namespace Library {

	class MaterialDeferredPLight : public Material {

		RTTI_DECLARATIONS(MaterialDeferredPLight, Material);

		MATERIAL_VARIABLES_DECLARATION(ScreenResolution, VP, CameraPosition, World,
									   LightColor, LightPosition, LightAttenuation,
									   PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	public:
		MaterialDeferredPLight();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}