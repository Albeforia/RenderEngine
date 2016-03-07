#pragma once

#include "Common.h"

struct aiMesh;

namespace Library {

	class Material;
	class ModelMaterial;

	class Mesh {

		friend class Model;

	public:
		Mesh(Model&, ModelMaterial*);
		~Mesh();

		Model& model();
		ModelMaterial* material() const;
		const std::string& name() const;

		const std::vector<XMFLOAT3>& vertices() const;
		const std::vector<XMFLOAT3>& normals() const;
		const std::vector<XMFLOAT3>& tangents() const;
		const std::vector<XMFLOAT3>& bi_normals() const;
		const std::vector<std::vector<XMFLOAT3>*>& texture_coords() const;
		const std::vector<std::vector<XMFLOAT4>*>& vertex_colors() const;
		UINT face_count() const;
		const std::vector<UINT>& indices() const;

		void create_index_buffer(ID3D11Buffer**);

	private:
		Mesh(Model&, aiMesh&);

		Model& m_model;
		ModelMaterial* m_material;
		std::string m_name;

		std::vector<XMFLOAT3> m_vertices;
		std::vector<XMFLOAT3> m_normals;
		std::vector<XMFLOAT3> m_tangents;
		std::vector<XMFLOAT3> m_bi_normals;
		std::vector<std::vector<XMFLOAT3>*> m_texture_coords;
		std::vector<std::vector<XMFLOAT4>*> m_vertex_colors;
		UINT m_face_count;
		std::vector<UINT> m_indices;

	};

}