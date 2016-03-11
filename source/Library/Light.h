#pragma once

#include "Common.h"
#include "GameComponent.h"

namespace Library {

	class Light : public GameComponent {

		RTTI_DECLARATIONS(Light, GameComponent);

	public:
		Light(Game&);
		virtual ~Light();

		const XMCOLOR& color() const;
		XMVECTOR colorv() const;
		FLOAT intensity() const;

		virtual void set_color(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
		virtual void set_color(XMCOLOR);
		virtual void set_color(FXMVECTOR);
		virtual void set_intensity(float);

	protected:
		XMCOLOR m_color;

	};

}