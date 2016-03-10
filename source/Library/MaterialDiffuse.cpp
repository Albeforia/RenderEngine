#include "MaterialDiffuse.h"
#include "GameException.h"
#include "Mesh.h"
#include "ColorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDiffuse);

	MaterialDiffuse::MaterialDiffuse()
		: MATERIAL_VARIABLES_INITIALIZATION(WVP, World, AmbientColor, LightColor, LightDirection, ColorTexture)
		Material("main11") {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDiffuse, WVP, World, AmbientColor, LightColor, LightDirection, ColorTexture);

	void MaterialDiffuse::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(WVP, World, AmbientColor, LightColor, LightDirection, ColorTexture);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			GENERATE_INPUT_ELEMENT_DESC(POSITION, TEXCOORD, NORMAL)
		};
		descs[0].AlignedByteOffset = 0;
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialDiffuse::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const auto& src_vertices = mesh.vertices();
		std::vector<XMFLOAT3>* tex_coords = mesh.texture_coords().at(0);
		assert(tex_coords->size() == src_vertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.normals();
		assert(normals.size() == src_vertices.size());

		std::vector<VertexMaterialDiffuse> vertices;
		vertices.reserve(src_vertices.size());
		for (UINT i = 0; i < src_vertices.size(); i++) {
			XMFLOAT3 pos = src_vertices.at(i);
			XMFLOAT3 uv = tex_coords->at(i);
			XMFLOAT3 n = normals.at(i);
			vertices.push_back(VertexMaterialDiffuse {
				XMFLOAT4(pos.x, pos.y, pos.z, 1.0f),
				XMFLOAT2(uv.x, uv.y),
				n
			});
		}
		Material::create_vertex_buffer(device, reinterpret_cast<void*>(&vertices[0]), vertices.size(), buffer);
	}

	UINT MaterialDiffuse::vertex_size() const {
		return sizeof(VertexMaterialDiffuse);
	}

}