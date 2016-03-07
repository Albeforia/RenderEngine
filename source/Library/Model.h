#pragma once

#include "Common.h"

namespace Library {

	class Game;
	class Mesh;
	class ModelMaterial;

	class Model {

	public:
		Model(Game&, const std::string& file, bool filp_uv = false);
		~Model();

		bool has_meshes() const;
		bool has_materials() const;

		const std::vector<Mesh*>& meshes() const;
		const std::vector<ModelMaterial*>& materials() const;

	private:
		Game& m_game;
		std::vector<Mesh*> m_meshes;
		std::vector<ModelMaterial*> m_materials;

	};

}