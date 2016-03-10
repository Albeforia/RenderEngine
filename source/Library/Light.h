#pragma once

#include "Common.h"
#include "GameComponent.h"

namespace Library {

	class Light : public GameComponent {

		RTTI_DECLARATIONS(Light, GameComponent);

	public:
		Light(Game& game);
		virtual ~Light();

		const XMCOLOR& color() const;
		XMVECTOR color_vector() const;
		void set_color(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
		void set_color(XMCOLOR);
		void set_color(FXMVECTOR);

	protected:
		XMCOLOR m_color;

	};

}