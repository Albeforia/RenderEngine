#include "TriangleDemo.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "ColorHelper.h"
#include "Camera.h"
#include "Utility.h"
#include <d3dcompiler.h>

namespace Rendering {

	RTTI_DEFINITIONS(TriangleDemo);

	TriangleDemo::TriangleDemo(Game& game, Camera& camera)
		: GameComponentDrawable(game, camera),
		m_effect {}, m_technique {}, m_pass {}, m_wvp {}, m_input_layout {},
		m_world_mat {MatrixHelper::Identity}, m_vertex_buffer {} {}

	TriangleDemo::~TriangleDemo() {
		ReleaseObject(m_wvp);
		ReleaseObject(m_pass);
		ReleaseObject(m_technique);
		ReleaseObject(m_effect);
		ReleaseObject(m_input_layout);
		ReleaseObject(m_vertex_buffer);
	}

	void TriangleDemo::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// compile the shader
		UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3D10Blob* compiled = nullptr;
		ID3D10Blob* err_msg = nullptr;
		HRESULT hr = D3DCompileFromFile(L"content\\effects\\basic_effect.fx",
										nullptr, nullptr, nullptr, "fx_5_0", flags, 0, &compiled, &err_msg);
		if (err_msg != nullptr) {
			GameException e(reinterpret_cast<char*>(err_msg->GetBufferPointer()), hr);
			throw e;
		}
		if (FAILED(hr)) {
			throw GameException("D3DCompileFromFile() failed.", hr);
		}

		// create effect object
		if (FAILED(hr = D3DX11CreateEffectFromMemory(compiled->GetBufferPointer(), compiled->GetBufferSize(),
													 0, m_game->d3d_device, &m_effect))) {
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}
		ReleaseObject(compiled);

		// get technique, pass and wvp from effect
		m_technique = m_effect->GetTechniqueByName("main11");
		if (m_technique == nullptr) {
			throw GameException("ID3DX11Effect::GetTechniqueByName() couldn't find the specified technique.");
		}
		m_pass = m_technique->GetPassByName("p0");
		if (m_pass == nullptr) {
			throw GameException("ID3DX11EffectTechnique::GetPassByName() couldn't find the specified pass.");
		}
		ID3DX11EffectVariable* v = m_effect->GetVariableByName("WVP");
		if (v == nullptr) {
			throw GameException("ID3DX11Effect::GetVariableByName() couldn't find the specified variable.");
		}
		m_wvp = v->AsMatrix();
		if (!m_wvp->IsValid()) {
			throw GameException("Invalid effect variable cast.");
		}

		// create input layout
		D3DX11_PASS_DESC pass_desc;
		m_pass->GetDesc(&pass_desc);
		D3D11_INPUT_ELEMENT_DESC input_ele_descs[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		if (FAILED(hr = m_game->d3d_device()->CreateInputLayout(input_ele_descs, ARRAYSIZE(input_ele_descs),
																pass_desc.pIAInputSignature, pass_desc.IAInputSignatureSize, &m_input_layout))) {
			throw GameException("ID3D11Device1::CreateInputLayout() failed.", hr);
		}

		// create vertex buffer
		BasicEffectVertex vertices[] = {
			BasicEffectVertex(XMFLOAT4(-1.0f,0.0f,0.0f,1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Red))),
			BasicEffectVertex(XMFLOAT4(0.0f,1.0f,0.0f,1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Green))),
			BasicEffectVertex(XMFLOAT4(1.0f,0.0f,0.0f,1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Blue)))
		};
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth = sizeof(BasicEffectVertex)*ARRAYSIZE(vertices);
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(sub_data));
		sub_data.pSysMem = vertices;
		if (FAILED(m_game->d3d_device()->CreateBuffer(&buffer_desc, &sub_data, &m_vertex_buffer))) {
			throw GameException("ID3D11Device1::CreateBuffer() failed.");
		}

	}

	void TriangleDemo::draw(const GameTime& game_time) {
		auto* con = m_game->d3d_device_context();
		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		con->IASetInputLayout(m_input_layout);
		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;
		con->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
		XMMATRIX world = XMLoadFloat4x4(&m_world_mat);
		XMMATRIX wvp = world * m_camera->view_projection();
		m_wvp->SetMatrix(reinterpret_cast<const float*>(&wvp));
		m_pass->Apply(0, con);
		con->Draw(3, 0);
	}

}