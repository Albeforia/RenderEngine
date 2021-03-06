#include "EarthDemo.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "MaterialDeferredGeometry.h"
#include <DDSTextureLoader.h>

namespace Rendering {

	RTTI_DEFINITIONS(EarthDemo);

	EarthDemo::EarthDemo(Game& game, Camera& camera, const std::wstring& file)
		: GameComponentDrawable(game, camera),
		m_texture_file {file}, m_texture {}, m_material {}, m_effect {},
		m_world_mat {MatrixHelper::Identity}, m_vertex_buffer {}, m_index_buffer {} {}

	EarthDemo::~EarthDemo() {
		ReleaseObject(m_texture);
		DeleteObject(m_material);
		DeleteObject(m_effect);
		ReleaseObject(m_index_buffer);
		ReleaseObject(m_vertex_buffer);
		ReleaseObject(m_instance_buffer);
		DeleteObject(m_buffers);
	}

	ID3D11ShaderResourceView* EarthDemo::texture() {
		return m_texture;
	}

	void EarthDemo::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		m_effect = new Effect(*m_game);
		m_effect->load(L"content\\effects\\deferred_geometry.cso");
		m_material = new MaterialDeferredGeometry();
		m_material->init(m_effect);
		m_material->set_curr_technique(1);

		std::unique_ptr<Model> model(new Model(*m_game, "content\\models\\Sphere.obj", true));
		Mesh* mesh = model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();

		// create instance buffer
		float scale = 5.0f;
		std::vector<XMFLOAT4X4> inst_data;
		UINT axis_count = 5;
		float offset = 20.0f;
		for (UINT x = 0; x < axis_count; x++) {
			float x_pox = x * offset;
			for (UINT z = 0; z < axis_count; z++) {
				float z_pos = z * offset;
				XMFLOAT4X4 m;
				XMStoreFloat4x4(&m, XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(-x_pox, 0, -z_pos));
				inst_data.push_back(m);
				XMStoreFloat4x4(&m, XMMatrixScaling(scale, scale, scale) * XMMatrixTranslation(+x_pox, 0, -z_pos));
				inst_data.push_back(m);
			}
		}
		m_instance_buffer = nullptr;
		m_instance_count = inst_data.size();
		m_material->create_buffer(m_game->d3d_device(), reinterpret_cast<void*>(&inst_data[0]),
								  m_instance_count, sizeof(XMFLOAT4X4),
								  D3D11_USAGE_IMMUTABLE, &m_instance_buffer);

		m_buffers = new ID3D11Buffer*[2] {m_vertex_buffer, m_instance_buffer};

		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_game->d3d_device(), m_texture_file.c_str(), nullptr, &m_texture);
		if (FAILED(hr)) {
			throw GameException("CreateDDSTextureFromFile() failed.", hr);
		}
	}

	void EarthDemo::draw(const GameTime& game_time) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const Pass* pass = m_material->curr_technique()->pass(0);
		ID3D11InputLayout* layout = pass->input_layout();
		con->IASetInputLayout(layout);

		UINT stride[2] = {m_material->vertex_size(), sizeof(XMFLOAT4X4)};
		UINT offset[2] = {0, 0};
		con->IASetVertexBuffers(0, 2, m_buffers, stride, offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX world = XMLoadFloat4x4(&m_world_mat);
		//XMMATRIX wvp = world * m_camera->view_projection();
		m_material->VP() << m_camera->view_projection();
		m_material->World() << world;
		m_material->DiffuseTexture() << m_texture;
		m_material->SpecularPower() << 25.0f;
		//m_material->SpecularTexture() << ;

		pass->apply(0, con);
		con->DrawIndexedInstanced(m_index_count, m_instance_count, 0, 0, 0);
	}

}