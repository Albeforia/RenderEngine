#include "Sphere.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "Utility.h"
#include "Effect.h"
#include "MaterialDeferredPLight.h"
#include "Camera.h"

namespace Library {

	RTTI_DEFINITIONS(Sphere);

	Sphere::Sphere(Game& game, Camera& camera, Material& material)
		: GameComponentDrawable(game, camera),
		m_material {material}, m_vertex_buffer {}, m_index_buffer {},
		m_index_count {}, m_update_material {} {}

	Sphere::~Sphere() {
		ReleaseObject(m_index_buffer);
		ReleaseObject(m_vertex_buffer);
	}

	void Sphere::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		std::unique_ptr<Model> model(new Model(*m_game, "content\\models\\Sphere.obj", true));
		Mesh* mesh = model->meshes().at(0);
		m_material.create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
	}

	void Sphere::set_update_material(std::function<void()> callback) {
		m_update_material = callback;
	}

	void Sphere::draw(const GameTime&) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const Pass* pass = m_material.curr_technique()->pass(0);
		ID3D11InputLayout* layout = pass->input_layout();
		con->IASetInputLayout(layout);

		UINT stride = m_material.vertex_size();
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		m_material.As<MaterialDeferredPLight>()->VP()
			<< m_camera->view_projection();
		m_material.As<MaterialDeferredPLight>()->CameraPosition()
			<< XMLoadFloat3(&m_camera->position());

		if (m_update_material != nullptr) {
			m_update_material();
		}

		pass->apply(0, con);
		con->DrawIndexed(m_index_count, 0, 0);
	}

}