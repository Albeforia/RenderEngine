#pragma once

#include "GameComponentDrawable.h"

using namespace Library;

namespace Library {
	class Effect;
	class MaterialDeferredGeometry;
}

namespace Rendering {

	class EarthDemo : public GameComponentDrawable {

		RTTI_DECLARATIONS(EarthDemo, GameComponentDrawable);

	public:
		EarthDemo(Game&, Camera&, const std::wstring& file);
		~EarthDemo();

		ID3D11ShaderResourceView* texture();

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		Effect* m_effect;
		MaterialDeferredGeometry* m_material;

		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

		std::wstring m_texture_file;
		ID3D11ShaderResourceView* m_texture;

		XMFLOAT4X4 m_world_mat;

	};

}