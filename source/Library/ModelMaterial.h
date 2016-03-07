#pragma once

#include "Common.h"

struct aiMaterial;

namespace Library {
	enum TextureType {
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

		ModelMaterial(Model&, aiMaterial*);

		Model& m_model;
		std::string m_name;
		std::map<TextureType, std::vector<std::wstring>*> m_textures;
	};
}