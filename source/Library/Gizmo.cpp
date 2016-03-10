#include "Gizmo.h"
#include "Game.h"
#include "GameException.h"
#include "MaterialBasic.h"
#include "Camera.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"
#include "Model.h"
#include "Mesh.h"
#include "Utility.h"
#include "RasterizerStates.h"

namespace Library {

	RTTI_DEFINITIONS(Gizmo);

	Gizmo::Gizmo(Game& game, Camera& camera, const std::string& file, float scale)
		: GameComponentDrawable(game, camera),
		m_file {file}, m_effect {}, m_material {},
		m_vertex_buffer {}, m_index_buffer {}, m_index_count {},
		m_world_matrix {MatrixHelper::Identity}, m_scale_matrix {MatrixHelper::Identity}, m_wireframe {true},
		m_position {Vector3Helper::Zero}, m_direction {Vector3Helper::Forward}, m_up {Vector3Helper::Up}, m_right {Vector3Helper::Right} {
		XMStoreFloat4x4(&m_scale_matrix, XMMatrixScaling(scale, scale, scale));
	}

	Gizmo::~Gizmo() {
		DeleteObject(m_material);
		DeleteObject(m_effect);
		ReleaseObject(m_vertex_buffer);
		ReleaseObject(m_index_buffer);
	}

	const XMFLOAT3& Gizmo::position() const {
		return m_position;
	}

	const XMFLOAT3& Gizmo::direction() const {
		return m_direction;
	}

	const XMFLOAT3& Gizmo::up() const {
		return m_up;
	}

	const XMFLOAT3& Gizmo::right() const {
		return m_right;
	}

	XMVECTOR Gizmo::positionv() const {
		return XMLoadFloat3(&m_position);
	}

	XMVECTOR Gizmo::directionv() const {
		return XMLoadFloat3(&m_direction);
	}

	XMVECTOR Gizmo::upv() const {
		return XMLoadFloat3(&m_up);
	}

	XMVECTOR Gizmo::rightv() const {
		return XMLoadFloat3(&m_right);
	}

	bool& Gizmo::wireframe() {
		return m_wireframe;
	}

	void Gizmo::set_position(FLOAT x, FLOAT y, FLOAT z) {
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		set_position(position);
	}

	void Gizmo::set_position(FXMVECTOR position) {
		XMStoreFloat3(&m_position, position);
	}

	void Gizmo::set_position(const XMFLOAT3& position) {
		m_position = position;
	}

	void Gizmo::apply_rotation(CXMMATRIX transform) {
		XMVECTOR direction = XMLoadFloat3(&m_direction);
		XMVECTOR up = XMLoadFloat3(&m_up);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&m_direction, direction);
		XMStoreFloat3(&m_up, up);
		XMStoreFloat3(&m_right, right);
	}

	void Gizmo::apply_rotation(const XMFLOAT4X4& transform) {
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		apply_rotation(transformMatrix);
	}

	void Gizmo::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		std::unique_ptr<Model> model(new Model(*m_game, m_file, true));

		m_effect = new Effect(*m_game);
		m_effect->load(L"Content\\Effects\\basic.cso");

		m_material = new MaterialBasic();
		m_material->init(m_effect);

		Mesh* mesh = model->meshes().at(0);
		m_material->create_vertex_buffer(m_game->d3d_device(), *mesh, &m_vertex_buffer);
		mesh->create_index_buffer(&m_index_buffer);
		m_index_count = mesh->indices().size();
	}

	void Gizmo::update(const GameTime& game_time) {
		XMMATRIX world_mat = XMMatrixIdentity();
		MatrixHelper::SetForward(world_mat, m_direction);
		MatrixHelper::SetUp(world_mat, m_up);
		MatrixHelper::SetRight(world_mat, m_right);
		MatrixHelper::SetTranslation(world_mat, m_position);
		XMStoreFloat4x4(&m_world_matrix, XMLoadFloat4x4(&m_scale_matrix) * world_mat);
	}

	void Gizmo::draw(const GameTime& gametime) {
		ID3D11DeviceContext* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = m_material->curr_technique()->passes().at(0);
		ID3D11InputLayout* inputLayout = m_material->input_layouts().at(pass);
		con->IASetInputLayout(inputLayout);

		UINT stride = m_material->vertex_size();
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		con->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX wvp = XMLoadFloat4x4(&m_world_matrix) * m_camera->view_projection();
		m_material->WVP() << wvp;

		pass->apply(0, con);

		if (m_wireframe) {
			// TODO batch Gizmo draw calls to reduce state switching
			con->RSSetState(RasterizerStates::Wireframe);
			con->DrawIndexed(m_index_count, 0, 0);
			con->RSSetState(nullptr);
		}
		else {
			con->DrawIndexed(m_index_count, 0, 0);
		}
	}

}