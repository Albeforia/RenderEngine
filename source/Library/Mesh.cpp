#include "Mesh.h"
#include "Model.h"
#include "Game.h"
#include "GameException.h"
#include <assimp\scene.h>

namespace Library {

	Mesh::Mesh(Model& model, aiMesh& mesh)
		: m_model {model}, m_material {}, m_name {mesh.mName.C_Str()},
		m_vertices {}, m_normals {}, m_tangents {}, m_bi_normals {},
		m_texture_coords {}, m_vertex_colors {}, m_face_count {}, m_indices {} {
		m_material = m_model.materials().at(mesh.mMaterialIndex);

		m_vertices.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			m_vertices.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mVertices[i])));
		}

		if (mesh.HasNormals()) {
			m_normals.reserve(mesh.mNumVertices);
			for (UINT i = 0; i < mesh.mNumVertices; i++) {
				m_normals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
			}
		}

		if (mesh.HasTangentsAndBitangents()) {
			m_tangents.reserve(mesh.mNumVertices);
			m_bi_normals.reserve(mesh.mNumVertices);
			for (UINT i = 0; i < mesh.mNumVertices; i++) {
				m_tangents.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
				m_bi_normals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mBitangents[i])));
			}
		}

		UINT uv_channels = mesh.GetNumUVChannels();
		for (UINT i = 0; i < uv_channels; i++) {
			auto* tex_coords = new std::vector<XMFLOAT3> {};
			tex_coords->reserve(mesh.mNumVertices);
			m_texture_coords.push_back(tex_coords);
			aiVector3D* ai_tex_coords = mesh.mTextureCoords[i];
			for (UINT j = 0; j < mesh.mNumVertices; j++) {
				tex_coords->push_back(XMFLOAT3(reinterpret_cast<const float*>(&ai_tex_coords[j])));
			}
		}

		UINT color_channels = mesh.GetNumColorChannels();
		for (UINT i = 0; i < color_channels; i++) {
			auto* vertex_colors = new std::vector<XMFLOAT4> {};
			vertex_colors->reserve(mesh.mNumVertices);
			m_vertex_colors.push_back(vertex_colors);
			aiColor4D* ai_vertex_colors = mesh.mColors[i];
			for (UINT j = 0; j < mesh.mNumVertices; j++) {
				vertex_colors->push_back(XMFLOAT4(reinterpret_cast<const float*>(&ai_vertex_colors[j])));
			}
		}

		if (mesh.HasFaces()) {
			m_face_count = mesh.mNumFaces;
			for (UINT i = 0; i < m_face_count; i++) {
				aiFace* face = &mesh.mFaces[i];
				for (UINT j = 0; j < face->mNumIndices; j++) {
					m_indices.push_back(face->mIndices[j]);
				}
			}
		}
	}

	Mesh::~Mesh() {
		for (auto* tex_coords : m_texture_coords) {
			DeleteObject(tex_coords);
		}
		for (auto* vertex_colors : m_vertex_colors) {
			DeleteObject(vertex_colors);
		}
	}

	void Mesh::create_index_buffer(ID3D11Buffer** buffer) {
		assert(buffer != nullptr);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(UINT)*m_indices.size();
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(sub_data));
		sub_data.pSysMem = &m_indices[0];
		if (FAILED(m_model.game().d3d_device()->CreateBuffer(&desc, &sub_data, buffer))) {
			throw GameException("ID3D11Device1::CreateBuffer() failed.");
		}
	}

}