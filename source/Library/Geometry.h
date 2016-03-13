#pragma once

#include "GameComponent.h"

namespace Library {

	class Mesh;

	/**
	 *  Wrapper around ID3D11Buffer(vertex and index).
	 *  Intent to share among multiple draw calls.
	 */
	class Geometry : public GameComponent {

		RTTI_DECLARATIONS(Geometry, GameComponent)

	public:
		Geometry(Game&);
		Geometry(Game&, Mesh&);

		virtual ~Geometry();

		virtual void init() override;

		UINT index_count() const;
		void apply() const;

	protected:
		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

		Mesh* m_mesh;

	};

}