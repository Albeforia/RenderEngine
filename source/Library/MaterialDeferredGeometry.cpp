#include "MaterialDeferredGeometry.h"
#include "Mesh.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredGeometry);

	MaterialDeferredGeometry::MaterialDeferredGeometry()
		: MATERIAL_VARIABLES_INITIALIZATION(WVP, World, DiffuseTexture, SpecularTexture)
		Material("main11") {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredGeometry, WVP, World, DiffuseTexture, SpecularTexture);

	void MaterialDeferredGeometry::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(WVP, World, DiffuseTexture, SpecularTexture);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			GENERATE_INPUT_ELEMENT_DESC(POSITION, NORMAL, TEXCOORD)
		};
		descs[0].AlignedByteOffset = 0;
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialDeferredGeometry::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const auto& src_vertices = mesh.vertices();
		const auto& normals = mesh.normals();
		assert(normals.size() == src_vertices.size());
		auto* tex_coords = mesh.texture_coords().at(0);
		assert(tex_coords->size() == src_vertices.size());
		std::vector<VertexMaterialDeferredGeometry> vertices;
		vertices.reserve(src_vertices.size());
		for (UINT i = 0; i < src_vertices.size(); i++) {
			XMFLOAT3 pos = src_vertices.at(i);
			XMFLOAT3 n = normals.at(i);
			XMFLOAT3 uv = tex_coords->at(i);
			vertices.push_back(VertexMaterialDeferredGeometry {
				XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), n, XMFLOAT2(uv.x, uv.y)
			});
		}
		Material::create_vertex_buffer(device, reinterpret_cast<void*>(&vertices[0]), vertices.size(), buffer);
	}

	UINT MaterialDeferredGeometry::vertex_size() const {
		return sizeof(VertexMaterialDeferredGeometry);
	}

}