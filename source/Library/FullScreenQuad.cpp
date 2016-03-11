#include "FullScreenQuad.h"
#include "Game.h"
#include "GameException.h"
#include "Material.h"

namespace Library {

	RTTI_DEFINITIONS(FullScreenQuad);

	FullScreenQuad::FullScreenQuad(Game& game)
		: GameComponentDrawable(game),
		m_material {}, m_pass {}, m_input_layout {},
		m_vertex_buffer {}, m_index_buffer {}, m_index_count {}, m_update_material {} {}

	FullScreenQuad::FullScreenQuad(Game& game, Material& material)
		: GameComponentDrawable(game),
		m_material {&material}, m_pass {}, m_input_layout {},
		m_vertex_buffer {}, m_index_buffer {}, m_index_count {}, m_update_material {} {}

	FullScreenQuad::~FullScreenQuad() {
		ReleaseObject(m_index_buffer);
		ReleaseObject(m_vertex_buffer);
	}

	Material* FullScreenQuad::material() {
		return m_material;
	}

	void FullScreenQuad::set_material(Material& material, const std::string& tname, const std::string& pname) {
		m_material = &material;
		set_active_technique(tname, pname);
	}

	void FullScreenQuad::set_active_technique(const std::string& tname, const std::string& pname) {
		Technique* technique = m_material->effect()->techniques_by_name().at(tname);
		assert(technique != nullptr);
		m_pass = technique->passes_by_name().at(pname);
		assert(m_pass != nullptr);
		m_input_layout = m_material->input_layouts().at(m_pass);
	}

	void FullScreenQuad::set_update_material(std::function<void()> callback) {
		m_update_material = callback;
	}

	void FullScreenQuad::init() {
		VertexQuad vertices[] =
		{
			VertexQuad { XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
			VertexQuad { XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			VertexQuad { XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
			VertexQuad { XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		};

		UINT indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};
		m_index_count = ARRAYSIZE(indices);

		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(VertexQuad) * ARRAYSIZE(vertices);
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

	void FullScreenQuad::draw(const GameTime&) {
		assert(m_pass != nullptr);
		assert(m_input_layout != nullptr);

		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		con->IASetInputLayout(m_input_layout);

		UINT stride = sizeof(VertexQuad);
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		if (m_update_material != nullptr) {
			m_update_material();
		}

		m_pass->apply(0, con);
		con->DrawIndexed(m_index_count, 0, 0);

		// reset resource
		ID3D11ShaderResourceView* null[3] = {};
		con->PSSetShaderResources(0, ARRAYSIZE(null), null);
	}

}