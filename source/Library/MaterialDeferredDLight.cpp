#include "MaterialDeferredDLight.h"
#include "GameException.h"
#include "Mesh.h"

namespace Library {

	RTTI_DEFINITIONS(MaterialDeferredDLight);

	MaterialDeferredDLight::MaterialDeferredDLight()
		: MATERIAL_VARIABLES_INITIALIZATION(AmbientColor, LightColor, LightDirection,
											CameraPosition, ScreenResolution,
											PositionBuffer, NormalBuffer, AlbedoSpecularBuffer)
		Material() {}

	MATERIAL_VARIABLES_DEFINITION(MaterialDeferredDLight, AmbientColor, LightColor, LightDirection,
								  CameraPosition, ScreenResolution,
								  PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);

	void MaterialDeferredDLight::init(Effect* effect) {
		Material::init(effect);
		MATERIAL_VARIABLES_RETRIEVE(AmbientColor, LightColor, LightDirection,
									CameraPosition, ScreenResolution,
									PositionBuffer, NormalBuffer, AlbedoSpecularBuffer);
	}

	void MaterialDeferredDLight::create_vertex_buffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** buffer) const {
		/*const auto& src_vertices = mesh.vertices();
		std::vector<XMFLOAT4> vertices;
		vertices.reserve(src_vertices.size());
		for (UINT i = 0; i < src_vertices.size(); i++) {
			XMFLOAT3 pos = src_vertices.at(i);
			vertices.push_back(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f));
		}
		Material::create_buffer(device, reinterpret_cast<void*>(&vertices[0]),
								vertices.size(), vertex_size(),
								D3D11_USAGE_IMMUTABLE, buffer);*/
	}

	UINT MaterialDeferredDLight::vertex_size() const {
		return sizeof(XMFLOAT4);
	}

}