#pragma once

#include "Common.h"

struct Assimp_Material;

namespace Library {
	enum class TextureType {
		TextureTypeDifffuse = 0,
		TextureTypeSpecularMap,
		TextureTypeAmbient,
		TextureTypeEmissive,
		TextureTypeHeightmap,
		TextureTypeNormalMap,
		TextureTypeSpecularPowerMap,
		TextureTypeDisplacementMap,
		TextureTypeLightMap,
		TextureTypeEnd
	};

	class ModelMaterial {
		friend class Model;

	public:
		ModelMaterial(Model&);
		~ModelMaterial();

		const std::string& name() const;
		const std::map<TextureType, std::vector<std::wstring>*> textures() const;

	private:
		static void InitTextureTypeMappings();
		static std::map<TextureType, UINT> TextureTypeMappings;

		ModelMaterial(Model&, Assimp_Material*);

		Model& m_model;
		std::string m_name;
		std::map<TextureType, std::vector<std::wstring>*> m_textures;
	};
}