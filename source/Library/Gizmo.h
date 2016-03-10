#pragma once

#pragma once

#include "Common.h"
#include "GameComponentDrawable.h"

namespace Library {

	class Effect;
	class MaterialBasic;

	class Gizmo : public GameComponentDrawable {

		RTTI_DECLARATIONS(Gizmo, GameComponentDrawable);

	public:
		Gizmo(Game&, Camera&, const std::string& file, float scale = 1.0f);
		~Gizmo();

		const XMFLOAT3& position() const;
		const XMFLOAT3& direction() const;
		const XMFLOAT3& up() const;
		const XMFLOAT3& right() const;
		XMVECTOR positionv() const;
		XMVECTOR directionv() const;
		XMVECTOR upv() const;
		XMVECTOR rightv() const;

		bool& wireframe();

		void set_position(FLOAT x, FLOAT y, FLOAT z);
		void set_position(FXMVECTOR);
		void set_position(const XMFLOAT3&);

		void apply_rotation(CXMMATRIX transform);
		void apply_rotation(const XMFLOAT4X4& transform);

		virtual void init() override;
		virtual void update(const GameTime&) override;
		virtual void draw(const GameTime&) override;

	private:
		std::string m_file;
		Effect* m_effect;
		MaterialBasic* m_material;
		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		UINT m_index_count;

		XMFLOAT4X4 m_world_matrix;
		XMFLOAT4X4 m_scale_matrix;

		bool m_wireframe;
		XMFLOAT3 m_position;
		XMFLOAT3 m_direction;
		XMFLOAT3 m_up;
		XMFLOAT3 m_right;
	};
}