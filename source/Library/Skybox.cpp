#include "Skybox.h"
#include "MatrixHelper.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "GameException.h"
#include "Camera.h"
#include <DDSTextureLoader.h>

namespace Library {

	RTTI_DEFINITIONS(Skybox);

	Skybox::Skybox(Game& game, Camera& camera, const std::wstring& cubemap_file, float scale)
		: GameComponentDrawable(game, camera),
		m_cubemap_file {cubemap_file}, m_effect {}, m_material {},
		m_cubemap_res_view {}, m_vertex_buffer {}, m_index_buffer {}, m_index_count {},
		m_world_mat {MatrixHelper::Identity}, m_scale_mat {MatrixHelper::Identity} {
		XMStoreFloat4x4(&m_scale_mat, XMMatrixScaling(scale, scale, scale));
	}

	Skybox::~Skybox() {
		ReleaseObject(m_cubemap_res_view);
		DeleteObject(m_material);
		DeleteObject(m_effect);
		ReleaseObject(m_vertex_buffer);
		ReleaseObject(m_index_buffer);
	}

	void Skybox::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		std::unique_ptr<Model> model(new Model(*m_game, "content\\models\\Sphere.obj", true));
		m_effect = new Effect(*m_game);
		m_effect->load(L"content\\effects\\skybox.cso");
		m_material = new MaterialSkybox();
		m_material->init(m_effect);
		Mesh* mesh = model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_game->d3d_device(), m_cubemap_file.c_str(), nullptr, &m_cubemap_res_view);
		if (FAILED(hr)) {
			throw GameException("CreateDDSTextureFromFile() failed.", hr);
		}
	}

	void Skybox::update(const GameTime& game_time) {
		const XMFLOAT3& pos = m_camera->position();
		XMStoreFloat4x4(&m_world_mat, XMLoadFloat4x4(&m_scale_mat) * XMMatrixTranslation(pos.x, pos.y, pos.z));
	}

	void Skybox::draw(const GameTime& game_time) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = m_material->curr_technique()->passes().at(0);
		ID3D11InputLayout* inputLayout = m_material->input_layouts().at(pass);
		con->IASetInputLayout(inputLayout);

		UINT stride = m_material->vertex_size();
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX wvp = XMLoadFloat4x4(&m_world_mat) * m_camera->view_projection();
		m_material->WVP() << wvp;
		m_material->SkyboxTexture() << m_cubemap_res_view;

		pass->apply(0, con);
		con->DrawIndexed(m_index_count, 0, 0);
	}

}