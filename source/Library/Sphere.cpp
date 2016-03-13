#include "Sphere.h"
#include "Game.h"
#include "GameException.h"
#include "Material.h"
#include "Model.h"
#include "Mesh.h"
#include "Utility.h"
#include "Effect.h"

namespace Library {

	RTTI_DEFINITIONS(Sphere);

	Sphere::Sphere(Game& game, Material& material)
		: GameComponentDrawable(game),
		m_material {&material}, m_vertex_buffer {}, m_index_buffer {}, m_index_count {},
		m_update_material {} {
		init();
		//set_material(material);
	}

	Sphere::~Sphere() {
		DeleteObject(m_model);
		ReleaseObject(m_index_buffer);
		ReleaseObject(m_vertex_buffer);
	}

	void Sphere::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		m_model = new Model(*m_game, "content\\models\\Sphere.obj", true);
		Mesh* mesh = m_model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
	}

	void Sphere::set_material(Material& material) {
		m_material = &material;
		Mesh* mesh = m_model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
	}

	void Sphere::set_update_material(std::function<void(const Sphere&)> callback) {
		m_update_material = callback;
	}

	void Sphere::draw(const GameTime&) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();

		// FIXME [WFT BUG] seems vertex buffer has some problem that
		// makes IA very strange, even if I only render this sphere
		// using different materials.
		// TEMP FIX: reuse VB from previous pass
		/*UINT stride = sizeof(m_material->vertex_size());
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);*/

		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		con->IASetInputLayout(m_material->curr_technique()->pass(0)->input_layout());

		/*if (m_update_material != nullptr) {
			m_update_material(*this);
		}*/

		m_material->curr_technique()->pass(0)->apply(0, con);
		con->DrawIndexed(m_index_count, 0, 0);
	}

}