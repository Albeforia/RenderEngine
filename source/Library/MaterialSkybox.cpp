#include "MaterialSkybox.h"
#include "Mesh.h"
#include "GameException.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialSkybox);

	MaterialSkybox::MaterialSkybox()
		: Material("main11"),
		MATERIAL_VARIABLE_INITIALIZATION(WVP),
		MATERIAL_VARIABLE_INITIALIZATION(SkyboxTexture) {}

	MATERIAL_VARIABLE_DEFINITION(MaterialSkybox, WVP);
	MATERIAL_VARIABLE_DEFINITION(MaterialSkybox, SkyboxTexture);

	void MaterialSkybox::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLE_RETRIEVE(WVP);
		MATERIAL_VARIABLE_RETRIEVE(SkyboxTexture);
		D3D11_INPUT_ELEMENT_DESC descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
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
		create_vertex_buffer(device, &vertices[0], vertices.size(), buffer);
	}

	void MaterialSkybox::create_vertex_buffer(ID3D11Device* device, XMFLOAT4* vertices, UINT count, ID3D11Buffer** buffer) const {
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = vertex_size() * count;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices;
		if (FAILED(device->CreateBuffer(&desc, &data, buffer))) {
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	UINT MaterialSkybox::vertex_size() const {
		return sizeof(XMFLOAT4);
	}

}