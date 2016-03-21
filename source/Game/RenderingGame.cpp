#include "RenderingGame.h"
#include "GameException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "RenderStateHelper.h"
#include "RenderTarget.h"
#include "CameraFirstPerson.h"
#include "ColorHelper.h"
#include "EarthDemo.h"
#include "Utility.h"
#include "Effect.h"
#include "MaterialDeferredStencil.h"
#include "MaterialDeferredDLight.h"
#include "MaterialDeferredPLight.h"
#include "MaterialBasic.h"
#include "MaterialDownSampling.h"
#include "MaterialGaussianBlur.h"
#include "MaterialDoF.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "Geometry.h"
#include "FullScreenQuad.h"
#include "Model.h"
#include "Mesh.h"

namespace Rendering {

	const XMVECTORF32 RenderingGame::BACKGROUND_COLOR = ColorHelper::Black;

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& window_class, const std::wstring& window_title, int show_cmd)
		: Game(instance, window_class, window_title, show_cmd),
		m_render_state_helper {}, m_render_targets {} {
		m_depth_stencil_enabled = false;
		// we have to disable MSAA because G-Buffers' MSAA count and
		// MSAA quality must equal to depth-stencil buffer's
		m_msaa_enabled = false;
	}

	RenderingGame::~RenderingGame() {}

	void RenderingGame::init() {
		// input
		if (FAILED(DirectInput8Create(m_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_input, nullptr))) {
			throw GameException("DirectInput8Create() failed.");
		}

		m_keyboard = new Keyboard(*this, m_input);
		m_components.push_back(m_keyboard);
		m_services.add_service(Keyboard::TypeIdClass(), m_keyboard);

		m_mouse = new Mouse(*this, m_input);
		m_components.push_back(m_mouse);
		m_services.add_service(Mouse::TypeIdClass(), m_mouse);

		// render
		m_render_state_helper = new RenderStateHelper(*this);
		auto* position = new RenderTarget(*this, true, true, DXGI_FORMAT_R32G32B32A32_FLOAT);
		auto* normal = new RenderTarget(*this, true, false, DXGI_FORMAT_R32G32B32A32_FLOAT);
		auto* albedo_specular = new RenderTarget(*this, true, false);
		auto* color = new RenderTarget(*this, true, false);
		auto* color_down = new RenderTarget(*this, true, false, DXGI_FORMAT_R8G8B8A8_UNORM, 4);
		auto* gaussian_blur_h = new RenderTarget(*this, true, false);
		auto* gaussian_blur_v = new RenderTarget(*this, true, false);
		m_render_targets.push_back(position);
		m_render_targets.push_back(normal);
		m_render_targets.push_back(albedo_specular);
		m_render_targets.push_back(color);
		m_render_targets.push_back(color_down);
		m_render_targets.push_back(gaussian_blur_h);
		m_render_targets.push_back(gaussian_blur_v);
		m_render_targets_raw = new ID3D11RenderTargetView*[m_render_targets.size()];
		for (UINT i = 0; i < m_render_targets.size(); i++) {
			m_render_targets_raw[i] = m_render_targets[i]->render_target();
		}

		// camera
		m_camera = new CameraFirstPerson(*this);
		m_components.push_back(m_camera);

		// scene
		m_components.push_back(new EarthDemo(*this, *m_camera, L"content\\textures\\Earth.dds"));

		// light
		m_sun = new LightDirectional(*this);
		m_components.push_back(m_sun);
		m_point_light = new LightPoint(*this);
		m_components.push_back(m_point_light);
		m_point_light->set_color(1.0f, 0, 0, 1.0f);
		m_point_light->As<LightPoint>()->set_attenuation(1.0f, 0.04f, 0.002f);
		m_point_light->As<LightPoint>()->set_position(0, 10.0f, -40.0f);

		// projectors
		/*m_projector = new Camera(*this);
		m_components.push_back(m_projector);*/

		// stencil
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		m_stencil_effect = new Effect(*this);
		m_stencil_effect->load(L"content\\effects\\deferred_stencil.cso");
		m_stencil_material = new MaterialDeferredStencil();
		m_stencil_material->init(m_stencil_effect);

		// point light volume
		m_light_effect = new Effect(*this);
		m_light_effect->load(L"content\\effects\\deferred_p_light.cso");
		m_light_material = new MaterialDeferredPLight();
		m_light_material->init(m_light_effect);
		m_light_material->As<MaterialDeferredPLight>()->
			ScreenResolution() << XMLoadFloat2(&XMFLOAT2(m_screen_width, m_screen_height));

		m_model = new Model(*this, "content\\models\\Sphere.obj", true);
		Mesh* mesh = m_model->meshes().at(0);
		m_sphere = new Geometry(*this, *mesh);
		m_components.push_back(m_sphere);

		// quad
		m_quad_effect = new Effect(*this);
		m_quad_effect->load(L"content\\effects\\deferred_d_light.cso");
		m_quad_material = new MaterialDeferredDLight();
		m_quad_material->init(m_quad_effect);
		m_quad_material->As<MaterialDeferredDLight>()->
			ScreenResolution() << XMLoadFloat2(&XMFLOAT2(m_screen_width, m_screen_height));

		m_quad = new FullScreenQuad(*this);
		m_components.push_back(m_quad);

		// test
		m_test_effect = new Effect(*this);
		m_test_effect->load(L"content\\effects\\basic.cso");
		m_test_material = new MaterialBasic();
		m_test_material->init(m_test_effect);
		m_test_material->set_curr_technique(1);

		// down-sampling
		m_down_sampling_effect = new Effect(*this);
		m_down_sampling_effect->load(L"content\\effects\\down_sampling.cso");
		m_down_sampling_material = new MaterialDownSampling(4);
		m_down_sampling_material->init(m_down_sampling_effect);

		// gaussian blur
		m_blur_effect = new Effect(*this);
		m_blur_effect->load(L"content\\effects\\gaussian_blur.cso");
		m_blur_material = new MaterialGaussianBlur(1.5f);
		m_blur_material->init(m_blur_effect);

		// dof
		m_dof_effect = new Effect(*this);
		m_dof_effect->load(L"content\\effects\\depth_of_field.cso");
		m_dof_material = new MaterialDoF(m_camera->As<CameraFirstPerson>());
		m_dof_material->init(m_dof_effect);

		// init each component
		Game::init();

		// after init
		m_camera->set_position(0, 0, 20.0f);
		//m_projector->set_position(m_point_light->As<LightPoint>()->positionv());

	}

	void RenderingGame::shutdown() {
		for (auto* t : m_render_targets) {
			DeleteObject(t);
		}
		DeleteObject(m_render_targets_raw);
		DeleteObject(m_render_state_helper);
		ReleaseObject(m_input);
		DeleteObject(m_stencil_effect);
		DeleteObject(m_stencil_material);
		DeleteObject(m_light_effect);
		DeleteObject(m_light_material);
		DeleteObject(m_quad_effect);
		DeleteObject(m_quad_material);
		DeleteObject(m_test_effect);
		DeleteObject(m_test_material);
		DeleteObject(m_down_sampling_effect);
		DeleteObject(m_down_sampling_material);
		DeleteObject(m_blur_effect);
		DeleteObject(m_blur_material);
		DeleteObject(m_dof_effect);
		DeleteObject(m_dof_material);
		DeleteObject(m_model);
		// keyboard, mouse and camera will be deleted with components
		Game::shutdown();
	}

	void RenderingGame::update(const GameTime& game_time) {
		Game::update(game_time);
	}

	void RenderingGame::draw(const GameTime& game_time) {
		//---------------------------------------------------
		// BEGIN: geometry pass
		m_d3d_device_context->OMSetRenderTargets(3, m_render_targets_raw, m_render_targets[0]->depth_stencil());
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[0], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[1], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[2], reinterpret_cast<const float*>(&BACKGROUND_COLOR));
		m_d3d_device_context->ClearDepthStencilView(m_render_targets[0]->depth_stencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		Game::draw(game_time);
		// END: geometry pass
		//---------------------------------------------------

		m_d3d_device_context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// use sphere mesh for next two passes
		m_sphere->apply();

		//---------------------------------------------------
		// BEGIN: stencil pass
		m_render_state_helper->save_all();

		m_d3d_device_context->ClearDepthStencilView(m_render_targets[0]->depth_stencil(), D3D11_CLEAR_STENCIL, 1.0f, 0);

		update_stencil_material();
		m_stencil_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_sphere->index_count(), 0, 0);

		m_render_state_helper->restore_all();
		// END: geometry pass
		//---------------------------------------------------

		//---------------------------------------------------
		// BEGIN: point light pass
		m_render_state_helper->save_all();

		m_d3d_device_context->OMSetRenderTargets(1, &m_render_targets_raw[3], m_render_targets[0]->depth_stencil());
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[3], reinterpret_cast<const float*>(&ColorHelper::Black));

		update_light_material();
		m_light_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_sphere->index_count(), 0, 0);

		m_render_state_helper->restore_all();
		// END: point light pass
		//---------------------------------------------------

		// use quad mesh from now on
		m_quad->apply();

		//---------------------------------------------------
		// BEGIN: directional light pass
		m_render_state_helper->save_all();

		update_quad_material();
		m_quad_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_quad->index_count(), 0, 0);

		m_render_state_helper->restore_all();
		// END: directional light pass
		//---------------------------------------------------

		//---------------------------------------------------
		// BEGIN: down-sampling (4x)

		// seems there's no need to change viewport...
		m_viewport.Width = static_cast<float>(m_screen_width) / 4.0f;
		m_viewport.Height = static_cast<float>(m_screen_height) / 4.0f;
		m_d3d_device_context->RSSetViewports(1, &m_viewport);

		m_d3d_device_context->OMSetRenderTargets(1, &m_render_targets_raw[4], nullptr);
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[4], reinterpret_cast<const float*>(&ColorHelper::Black));
		m_down_sampling_material->As<MaterialDownSampling>()->ColorBuffer() << m_render_targets[3]->output_texture();
		m_down_sampling_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_quad->index_count(), 0, 0);

		m_viewport.Width = static_cast<float>(m_screen_width);
		m_viewport.Height = static_cast<float>(m_screen_height);
		m_d3d_device_context->RSSetViewports(1, &m_viewport);

		// END: down-sampling (4x)
		//---------------------------------------------------

		//---------------------------------------------------
		// BEGIN: post-processing

		// horizontal gaussian blur
		m_d3d_device_context->OMSetRenderTargets(1, &m_render_targets_raw[5], nullptr);
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[5], reinterpret_cast<const float*>(&ColorHelper::Black));
		m_blur_material->set_curr_technique(0);
		m_blur_material->As<MaterialGaussianBlur>()->ColorBuffer() << m_render_targets[4]->output_texture();
		m_blur_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_quad->index_count(), 0, 0);

		// vertical gaussian blur
		m_d3d_device_context->OMSetRenderTargets(1, &m_render_targets_raw[6], nullptr);
		m_d3d_device_context->ClearRenderTargetView(m_render_targets_raw[6], reinterpret_cast<const float*>(&ColorHelper::Black));
		m_blur_material->set_curr_technique(1);
		m_blur_material->As<MaterialGaussianBlur>()->ColorBuffer() << m_render_targets[5]->output_texture();
		m_blur_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_quad->index_count(), 0, 0);

		// dof
		m_d3d_device_context->OMSetRenderTargets(1, &m_render_target_back, nullptr);
		m_d3d_device_context->ClearRenderTargetView(m_render_target_back, reinterpret_cast<const float*>(&ColorHelper::Black));
		m_dof_material->As<MaterialDoF>()->ColorBuffer() << m_render_targets[3]->output_texture();
		m_dof_material->As<MaterialDoF>()->BlurBuffer() << m_render_targets[6]->output_texture();
		m_dof_material->As<MaterialDoF>()->DepthBuffer() << m_render_targets[0]->output_depth_texture();
		m_dof_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_quad->index_count(), 0, 0);

		// END: post-processing
		//---------------------------------------------------

		// reset resource
		ID3D11ShaderResourceView* null[5] = {};
		m_d3d_device_context->PSSetShaderResources(0, ARRAYSIZE(null), null);

		// test
		/*
		m_render_state_helper->save_all();
		m_sphere->apply();
		MaterialBasic* m = m_test_material->As<MaterialBasic>();
		LightPoint* l = m_point_light->As<LightPoint>();
		float r = l->radius();
		XMMATRIX world = XMMatrixScaling(r, r, r) * XMMatrixTranslationFromVector(l->positionv());
		m->WVP() << world * m_camera->view_projection();
		m_test_material->apply(m_d3d_device_context);
		m_d3d_device_context->DrawIndexed(m_sphere->index_count(), 0, 0);
		m_render_state_helper->restore_all();
		*/

		// swap
		HRESULT hr = m_swap_chain->Present(0, 0);
		if (FAILED(hr)) {
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}

	void RenderingGame::update_stencil_material() {
		MaterialDeferredStencil* m = m_stencil_material->As<MaterialDeferredStencil>();
		LightPoint* l = m_point_light->As<LightPoint>();
		float r = l->radius();
		XMMATRIX world = XMMatrixScaling(r, r, r) * XMMatrixTranslationFromVector(l->positionv());
		m->WVP() << world * m_camera->view_projection();
	}

	void RenderingGame::update_light_material() {
		MaterialDeferredPLight* m = m_light_material->As<MaterialDeferredPLight>();
		m->VP() << m_camera->view_projection();
		m->CameraPosition() << XMLoadFloat3(&m_camera->position());
		//m->ScreenResolution() << XMLoadFloat2(&XMFLOAT2(m_screen_width, m_screen_height));
		LightPoint* l = m_point_light->As<LightPoint>();
		XMVECTOR pos = l->positionv();
		float r = l->radius();
		m->World() << XMMatrixScaling(r, r, r) * XMMatrixTranslationFromVector(pos);
		m->LightColor() << l->colorv();
		m->LightPosition() << pos;
		m->LightAttenuation() << l->attenuation();
		//
		m->PositionBuffer() << m_render_targets[0]->output_texture();
		m->NormalBuffer() << m_render_targets[1]->output_texture();
		m->AlbedoSpecularBuffer() << m_render_targets[2]->output_texture();
	}

	void RenderingGame::update_quad_material() {
		MaterialDeferredDLight* m = m_quad_material->As<MaterialDeferredDLight>();
		//m->ScreenResolution() << XMLoadFloat2(&XMFLOAT2(m_screen_width, m_screen_height));
		m->CameraPosition() << XMLoadFloat3(&m_camera->position());
		m->AmbientColor() << DirectX::operator*(0.2f, ColorHelper::White);
		LightDirectional* l = m_sun->As<LightDirectional>();
		m->LightColor() << l->colorv();
		m->LightDirection() << l->directionv();
		//
		m->PositionBuffer() << m_render_targets[0]->output_texture();
		m->NormalBuffer() << m_render_targets[1]->output_texture();
		m->AlbedoSpecularBuffer() << m_render_targets[2]->output_texture();
	}

}