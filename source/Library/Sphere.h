#pragma once

#include <functional>
#include "GameComponentDrawable.h"

namespace Library {

	class Material;

	class Sphere : public GameComponentDrawable {

		RTTI_DECLARATIONS(Sphere, GameComponentDrawable)

	public:
		Sphere(Game&, Camera&, Material&);
		~Sphere();

		void set_update_material(std::function<void()>);

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		Material& m_material;

		std::function<void()> m_update_material;

		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

	};

}