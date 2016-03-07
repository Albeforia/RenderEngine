#pragma once

#include "GameComponentDrawable.h"

using namespace Library;

namespace Library {
	class Effect;
	class MaterialBasic;
}

namespace Rendering {

	class MaterialBasicDemo : public GameComponentDrawable {

		RTTI_DECLARATIONS(MaterialBasicDemo, GameComponentDrawable);

	public:
		MaterialBasicDemo(Game&, Camera&);
		~MaterialBasicDemo();

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		Effect* m_effect;
		MaterialBasic* m_material;
		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

		XMFLOAT4X4 m_world_mat;

	};

}