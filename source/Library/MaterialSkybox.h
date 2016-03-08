#pragma once

#include "Common.h"
#include "Material.h"

namespace Library {

	class MaterialSkybox : public Material {

		RTTI_DECLARATIONS(MaterialSkybox, Material);

		MATERIAL_VARIABLE_DECLARATION(WVP);
		MATERIAL_VARIABLE_DECLARATION(SkyboxTexture);

	public:
		MaterialSkybox();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual void create_vertex_buffer(ID3D11Device*, XMFLOAT4* vertices, UINT count, ID3D11Buffer**) const;
		virtual UINT vertex_size() const override;

	};

}