#pragma once

#include "GameComponent.h"

namespace Library {

	class GameTime;

	class Camera : public GameComponent {

		RTTI_DECLARATIONS(Camera, GameComponent);

	public:
		Camera(Game&);
		Camera(Game&, float nearp, float farp);
		virtual ~Camera();

		const XMFLOAT3& position() const;
		const XMFLOAT3& direction() const;
		const XMFLOAT3& up() const;
		const XMFLOAT3& right() const;

		float nearp() const;
		float farp() const;

		XMMATRIX view_projection() const;

		virtual void set_position(FLOAT x, FLOAT y, FLOAT z);
		virtual void set_position(FXMVECTOR);
		virtual void set_position(const XMFLOAT3&);

		virtual void reset();
		virtual void init() override;
		virtual void update(const GameTime&) override;
		virtual void update_view();
		virtual void update_projection() = 0;

		void apply_rotation(CXMMATRIX);
		void apply_rotation(const XMFLOAT4X4&);

		static const float DEFAULT_NEAR;
		static const float DEFAULT_FAR;

	protected:
		float m_nearp;
		float m_farp;

		XMFLOAT3 m_position;
		XMFLOAT3 m_direction;
		XMFLOAT3 m_up;
		XMFLOAT3 m_right;

		XMFLOAT4X4 m_view;
		XMFLOAT4X4 m_projection;

	};

}