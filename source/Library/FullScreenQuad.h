#pragma once

#include <functional>
#include "GameComponentDrawable.h"
#include "EffectHelper.h"

namespace Library {

	class Material;
	class Effect;
	class Pass;

	VERTEX_STRUCT_DECLARATION(Quad, POSITION, TEXCOORD)

	class FullScreenQuad : public GameComponentDrawable {

		RTTI_DECLARATIONS(FullScreenQuad, GameComponentDrawable)

	public:
		FullScreenQuad(Game&);
		FullScreenQuad(Game&, Material&);
		~FullScreenQuad();

		Material* material();
		void set_material(Material&, const std::string& technique, const std::string& pass);
		void set_active_technique(const std::string& technique, const std::string& pass);
		void set_update_material(std::function<void()>);

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		Material* m_material;
		Pass* m_pass;
		ID3D11InputLayout* m_input_layout;

		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;
		std::function<void()> m_update_material;
	};

}