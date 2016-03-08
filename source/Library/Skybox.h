#pragma once

#include "GameComponentDrawable.h"
#include "MaterialSkybox.h"

namespace Library {

	class Effect;

	class Skybox : public GameComponentDrawable {

		RTTI_DECLARATIONS(Skybox, GameComponentDrawable);

	public:
		Skybox(Game&, Camera&, const std::wstring& cubemap_file, float scale);
		~Skybox();

		virtual void init() override;
		virtual void update(const GameTime&) override;
		virtual void draw(const GameTime&) override;

	private:
		std::wstring m_cubemap_file;
		Effect* m_effect;
		MaterialSkybox* m_material;
		ID3D11ShaderResourceView* m_cubemap_res_view;
		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

		XMFLOAT4X4 m_world_mat;
		XMFLOAT4X4 m_scale_mat;

	};

}