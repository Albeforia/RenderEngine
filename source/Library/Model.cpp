#include "Model.h"
#include "Game.h"
#include "GameException.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace Library {

	Model::Model(Game& game, const std::string& file, bool flip_uv)
		: m_game {game}, m_meshes {}, m_materials {} {
		Assimp::Importer importer;
		UINT flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
		if (flip_uv) {
			flags |= aiProcess_FlipUVs;
		}
		const aiScene* scene = importer.ReadFile(file, flags);
		if (scene == nullptr) {
			throw GameException(importer.GetErrorString());
		}
		if (scene->HasMaterials()) {
			for (UINT i = 0; i < scene->mNumMaterials; i++) {
				m_materials.push_back(new ModelMaterial(*this, scene->mMaterials[i]));
			}
		}
		if (scene->HasMeshes()) {
			for (UINT i = 0; i < scene->mNumMeshes; i++) {
				m_meshes.push_back(new Mesh(*this, *(scene->mMeshes[i])));
			}
		}
	}

	Model::~Model() {
		for (auto* m : m_meshes) {
			DeleteObject(m);
		}
		for (auto* m : m_materials) {
			DeleteObject(m);
		}
	}

	Game& Model::game() {
		return m_game;
	}

	const std::vector<Mesh*>& Model::meshes() const {
		return m_meshes;
	}

	const std::vector<ModelMaterial*>& Model::materials() const {
		return m_materials;
	}

	bool Model::has_meshes() const {
		return m_meshes.size() > 0;
	}

	bool Model::has_materials() const {
		return m_materials.size() > 0;
	}

}