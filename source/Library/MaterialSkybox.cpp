#include "MaterialSkybox.h"
#include "Mesh.h"
#include "GameException.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialSkybox);

	MaterialSkybox::MaterialSkybox()
		: MATERIAL_VARIABLES_INITIALIZATION(WVP, SkyboxTexture)
		Material("main11") {}

	MATERIAL_VARIABLES_DEFINITION(MaterialSkybox, WVP, SkyboxTexture);

	void MaterialSkybox::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(WVP, SkyboxTexture);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			GENERATE_INPUT_ELEMENT_DESC(POSITION)
		};
		descs[0].AlignedByteOffset = 0;
		create_input_layout("main11", "p0", descs, ARRAYSIZE(descs));
	}

	void MaterialSkybox::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		const std::vector<XMFLOAT3>& src = mesh.vertices();
		std::vector<XMFLOAT4> vertices;
		vertices.reserve(src.size());
		for (UINT i = 0; i < src.size(); i++) {
			XMFLOAT3 position = src.at(i);
			vertices.push_back(XMFLOAT4(position.x, position.y, position.z, 1.0f));
		}
		Material::create_vertex_buffer(device, reinterpret_cast<void*>(&vertices[0]), vertices.size(), buffer);
	}

	UINT MaterialSkybox::vertex_size() const {
		return sizeof(XMFLOAT4);
	}

}