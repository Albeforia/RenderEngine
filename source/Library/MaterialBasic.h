#pragma once

#include "Common.h"
#include "Material.h"

namespace Library {

	VERTEX_STRUCT_DECLARATION(MaterialBasic, POSITION, COLOR)

	class MaterialBasic : public Material {

		RTTI_DECLARATIONS(MaterialBasic, Material);

		MATERIAL_VARIABLES_DECLARATION(WVP);

	public:
		MaterialBasic();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual UINT vertex_size() const override;

	};

}