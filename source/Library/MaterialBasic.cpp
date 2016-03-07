#include "MaterialBasic.h"
#include "GameException.h"
#include "Mesh.h"
#include "ColorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialBasic);

	MaterialBasic::MaterialBasic()
		: Material("main11"), MATERIAL_VARIABLE_INITIALIZATION(WVP) {}

	MATERIAL_VARIABLE_DEFINITION(MaterialBasic, WVP);

	void MaterialBasic::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLE_RETRIEVE(WVP);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialBasic::create_vertex_buffer(ID3D11Device* dev, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const auto& src_vertices = mesh.vertices();
		std::vector<VertexMaterialBasic> vertices;
		vertices.reserve(src_vertices.size());
		if (mesh.vertex_colors().size() > 0) {
			auto* vertex_colors = mesh.vertex_colors().at(0);
			assert(vertex_colors->size() == src_vertices.size());
			for (UINT i = 0; i < src_vertices.size(); i++) {
				XMFLOAT3 pos = src_vertices.at(i);
				XMFLOAT4 color = vertex_colors->at(i);
				vertices.push_back(VertexMaterialBasic(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), color));
			}
		}
		else {
			XMFLOAT4 color = XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::White));
			for (UINT i = 0; i < src_vertices.size(); i++) {
				XMFLOAT3 pos = src_vertices.at(i);
				vertices.push_back(VertexMaterialBasic(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), color));
			}
		}
		create_vertex_buffer(dev, &vertices[0], vertices.size(), buffer);
	}

	void MaterialBasic::create_vertex_buffer(ID3D11Device* dev, VertexMaterialBasic* vertices, UINT count, ID3D11Buffer** buffer) const {
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = vertex_size()*count;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(sub_data));
		sub_data.pSysMem = vertices;
		if (FAILED(dev->CreateBuffer(&desc, &sub_data, buffer))) {
			throw GameException("ID3D11Device1::CreateBuffer() failed.");
		}
	}

	UINT MaterialBasic::vertex_size() const {
		return sizeof(VertexMaterialBasic);
	}

}