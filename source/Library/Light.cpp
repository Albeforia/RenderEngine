#include "Light.h"
#include "ColorHelper.h"

namespace Library {

	RTTI_DEFINITIONS(Light);

	Light::Light(Game& game)
		: GameComponent(game), m_color(reinterpret_cast<const float*>(&ColorHelper::White)) {}

	Light::~Light() {}

	const XMCOLOR& Light::color() const {
		return m_color;
	}

	XMVECTOR Light::color_vector() const {
		return XMLoadColor(&m_color);
	}

	void Light::set_color(FLOAT r, FLOAT g, FLOAT b, FLOAT a) {
		XMCOLOR color(r, g, b, a);
		set_color(color);
	}

	void Light::set_color(XMCOLOR color) {
		m_color = color;
	}

	void Light::set_color(FXMVECTOR color) {
		XMStoreColor(&m_color, color);
	}

}