#include "FullScreenQuad.h"
#include "Game.h"
#include "GameException.h"

namespace Library {

	RTTI_DEFINITIONS(FullScreenQuad);

	FullScreenQuad::FullScreenQuad(Game& game)
		: Geometry(game) {}

	void FullScreenQuad::init() {
		XMFLOAT4 vertices[] = {
			XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),
			XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
			XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),
		};
		UINT indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		m_index_count = ARRAYSIZE(indices);

		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(XMFLOAT4) * ARRAYSIZE(vertices);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices;
		if (FAILED(m_game->d3d_device()->CreateBuffer(&desc, &data, &m_vertex_buffer))) {
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}

		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(UINT) * m_index_count;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices;
		if (FAILED(m_game->d3d_device()->CreateBuffer(&desc, &data, &m_index_buffer))) {
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

}