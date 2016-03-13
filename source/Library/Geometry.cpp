#include "Geometry.h"
#include "Game.h"
#include "GameException.h"
#include "Mesh.h"

namespace Library {

	RTTI_DEFINITIONS(Geometry);

	Geometry::Geometry(Game& game)
		: GameComponent(game), m_mesh {},
		m_vertex_buffer {}, m_index_buffer {}, m_index_count {} {}

	Geometry::Geometry(Game& game, Mesh& mesh)
		: GameComponent(game), m_mesh {&mesh},
		m_vertex_buffer {}, m_index_buffer {}, m_index_count {} {}

	Geometry::~Geometry() {
		ReleaseObject(m_vertex_buffer);
		ReleaseObject(m_index_buffer);
	}

	void Geometry::init() {
		if (m_mesh != nullptr) {
			const auto& src_vertices = m_mesh->vertices();
			std::vector<XMFLOAT4> vertices;
			vertices.reserve(src_vertices.size());
			for (UINT i = 0; i < src_vertices.size(); i++) {
				XMFLOAT3 pos = src_vertices.at(i);
				vertices.push_back(XMFLOAT4(pos.x, pos.y, pos.z, 1.0f));
			}
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ByteWidth = sizeof(XMFLOAT4) * vertices.size();
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA sub;
			ZeroMemory(&sub, sizeof(sub));
			sub.pSysMem = &vertices[0];
			if (FAILED(m_game->d3d_device()->CreateBuffer(&desc, &sub, &m_vertex_buffer))) {
				throw GameException("ID3D11Device::CreateBuffer() failed.");
			}
			m_mesh->create_index_buffer(&m_index_buffer);
			m_index_count = m_mesh->indices().size();
		}
	}

	UINT Geometry::index_count() const {
		return m_index_count;
	}

	void Geometry::apply() const {
		UINT stride = sizeof(XMFLOAT4);
		UINT offset = 0;
		m_game->d3d_device_context()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		m_game->d3d_device_context()->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
	}

}