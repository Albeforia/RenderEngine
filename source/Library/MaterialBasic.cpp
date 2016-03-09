#include "MaterialBasic.h"
#include "GameException.h"
#include "Mesh.h"
#include "ColorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialBasic);

	MaterialBasic::MaterialBasic()
		: MATERIAL_VARIABLES_INITIALIZATION(WVP)
		Material("main11") {}

	MATERIAL_VARIABLES_DEFINITION(MaterialBasic, WVP);

	void MaterialBasic::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(WVP);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			GENERATE_INPUT_ELEMENT_DESC(POSITION, COLOR)
		};
		descs[0].AlignedByteOffset = 0;
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialBasic::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const auto& src_vertices = mesh.vertices();
		std::vector<VertexMaterialBasic> vertices;
		vertices.reserve(src_vertices.size());
		if (mesh.vertex_colors().size() > 0) {
			auto* vertex_colors = mesh.vertex_colors().at(0);
			assert(vertex_colors->size() == src_vertices.size());
			for (UINT i = 0; i < src_vertices.size(); i++) {
				XMFLOAT3 pos = src_vertices.at(i);
				XMFLOAT4 color = vertex_colors->at(i);
				vertices.push_back(VertexMaterialBasic {
					XMFLOAT4(pos.x, pos.y, pos.z, 1.0f),
					color
				});
			}
		}
		else {
			XMFLOAT4 color = XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::White));
			for (UINT i = 0; i < src_vertices.size(); i++) {
				XMFLOAT3 pos = src_vertices.at(i);
				vertices.push_back(VertexMaterialBasic {
					XMFLOAT4(pos.x, pos.y, pos.z, 1.0f),
					color
				});
			}
		}
		Material::create_vertex_buffer(device, reinterpret_cast<void*>(&vertices[0]), vertices.size(), buffer);
	}

	UINT MaterialBasic::vertex_size() const {
		return sizeof(VertexMaterialBasic);
	}

}