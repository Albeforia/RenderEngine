#pragma once

#include "Common.h"
#include "Material.h"

namespace Library {

	typedef struct _VertexMaterialBasic {
		XMFLOAT4 position;
		XMFLOAT4 color;

		_VertexMaterialBasic() {}
		_VertexMaterialBasic(const XMFLOAT4& p, const XMFLOAT4& c)
			: position {p}, color {c} {}
	} VertexMaterialBasic;

	class MaterialBasic : public Material {

		RTTI_DECLARATIONS(MaterialBasic, Material);

		MATERIAL_VARIABLE_DECLARATION(WVP);

	public:
		MaterialBasic();

		virtual void init(Effect*) override;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const override;
		virtual void create_vertex_buffer(ID3D11Device*, VertexMaterialBasic*, UINT count, ID3D11Buffer**) const;
		virtual UINT vertex_size() const override;

	};

}