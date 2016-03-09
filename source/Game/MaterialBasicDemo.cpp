#include "MaterialBasicDemo.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "MaterialBasic.h"

namespace Rendering {

	RTTI_DEFINITIONS(MaterialBasicDemo);

	MaterialBasicDemo::MaterialBasicDemo(Game& game, Camera& camera)
		: GameComponentDrawable(game, camera), m_material {}, m_effect {},
		m_world_mat {MatrixHelper::Identity}, m_vertex_buffer {}, m_index_buffer {} {}

	MaterialBasicDemo::~MaterialBasicDemo() {
		DeleteObject(m_material);
		DeleteObject(m_effect);
		ReleaseObject(m_vertex_buffer);
		ReleaseObject(m_index_buffer);
	}

	void MaterialBasicDemo::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		std::unique_ptr<Model> model(new Model(*m_game, "content\\models\\Sphere.obj", true));
		m_effect = new Effect(*m_game);
		m_effect->load(L"content\\effects\\basic.cso");
		m_material = new MaterialBasic();
		m_material->init(m_effect);

		Mesh* mesh = model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
	}

	void MaterialBasicDemo::draw(const GameTime& game_time) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Pass* pass = m_material->curr_technique()->passes().at(0);
		ID3D11InputLayout* layout = m_material->input_layouts().at(pass);
		con->IASetInputLayout(layout);
		UINT stride = m_material->vertex_size();
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
		XMMATRIX world = XMLoadFloat4x4(&m_world_mat);
		XMMATRIX wvp = world*m_camera->view_projection();
		m_material->WVP() << wvp;
		pass->apply(0, con);
		con->DrawIndexed(m_index_count, 0, 0);
	}

}