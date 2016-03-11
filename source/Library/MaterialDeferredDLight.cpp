#include "MaterialDeferredDLight.h"
#include "GameException.h"
#include "Mesh.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredDLight);

	MaterialDeferredDLight::MaterialDeferredDLight()
		: MATERIAL_VARIABLES_INITIALIZATION(AmbientColor, LightColor, LightDirection, CameraPosition, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer)
		Material("main11") {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredDLight, AmbientColor, LightColor, LightDirection, CameraPosition, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	void MaterialDeferredDLight::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(AmbientColor, LightColor, LightDirection, CameraPosition, PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			GENERATE_INPUT_ELEMENT_DESC(POSITION, TEXCOORD)
		};
		descs[0].AlignedByteOffset = 0;
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialDeferredDLight::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const auto& src_vertices = mesh.vertices();
		std::vector<XMFLOAT3>* tex_coords = mesh.texture_coords().at(0);
		assert(tex_coords->size() == src_vertices.size());
		std::vector<VertexMaterialDeferredDLight> vertices;
		vertices.reserve(src_vertices.size());
		for (UINT i = 0; i < src_vertices.size(); i++) {
			XMFLOAT3 pos = src_vertices.at(i);
			XMFLOAT3 uv = tex_coords->at(i);
			vertices.push_back(VertexMaterialDeferredDLight {
				XMFLOAT4(pos.x, pos.y, pos.z, 1.0f), XMFLOAT2(uv.x, uv.y),
			});
		}
		Material::create_vertex_buffer(device, reinterpret_cast<void*>(&vertices[0]), vertices.size(), buffer);
	}

	UINT MaterialDeferredDLight::vertex_size() const {
		return sizeof(VertexMaterialDeferredDLight);
	}

}