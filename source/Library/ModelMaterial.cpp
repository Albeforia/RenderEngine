#include "ModelMaterial.h"
#include "GameException.h"
#include "Utility.h"
#include <assimp\scene.h>

namespace Library {

	std::map<TextureType, UINT> ModelMaterial::TextureTypeMappings;

	ModelMaterial::ModelMaterial(Model& model)
		: m_model {model}, m_textures {} {
		InitTextureTypeMappings();
	}

	ModelMaterial::ModelMaterial(Model& model, aiMaterial* material)
		: m_model {model}, m_textures {} {
		InitTextureTypeMappings();
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		m_name = name.C_Str();
		for (TextureType type = TextureType(0); type < TextureTypeEnd; type = TextureType(type + 1)) {
			aiTextureType mapped_type = aiTextureType(TextureTypeMappings[type]);
			UINT tex_count = material->GetTextureCount(mapped_type);
			if (tex_count > 0) {
				auto* textures = new std::vector<std::wstring> {};
				m_textures.insert({type, textures});
				for (UINT i = 0; i < tex_count; i++) {
					aiString path;
					if (material->GetTexture(mapped_type, i, &path) == AI_SUCCESS) {
						std::wstring w_path;
						Utility::ToWideString(path.C_Str(), w_path);
						textures->push_back(w_path);
					}
				}
			}
		}
	}

	ModelMaterial::~ModelMaterial() {
		for (auto t : m_textures) {
			DeleteObject(t.second);
		}
	}

	void ModelMaterial::InitTextureTypeMappings() {
		if (TextureTypeMappings.size() != TextureTypeEnd) {
			TextureTypeMappings[TextureTypeDifffuse] = aiTextureType_DIFFUSE;
			TextureTypeMappings[TextureTypeSpecularMap] = aiTextureType_SPECULAR;
			TextureTypeMappings[TextureTypeAmbient] = aiTextureType_AMBIENT;
			TextureTypeMappings[TextureTypeHeightmap] = aiTextureType_HEIGHT;
			TextureTypeMappings[TextureTypeNormalMap] = aiTextureType_NORMALS;
			TextureTypeMappings[TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
			TextureTypeMappings[TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
			TextureTypeMappings[TextureTypeLightMap] = aiTextureType_LIGHTMAP;
		}
	}

}