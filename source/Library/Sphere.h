#pragma once

#include <functional>
#include "GameComponentDrawable.h"

namespace Library {

	class Material;
	class Model;

	class Sphere : public GameComponentDrawable {

		RTTI_DECLARATIONS(Sphere, GameComponentDrawable)

	public:
		Sphere(Game&, Material&);
		~Sphere();

		void set_material(Material&);
		void set_update_material(std::function<void(const Sphere&)>);

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		Model* m_model;

		Material* m_material;
		std::function<void(const Sphere&)> m_update_material;

		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

	};

}