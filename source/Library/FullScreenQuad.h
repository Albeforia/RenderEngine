#pragma once

#include "Geometry.h"

namespace Library {

	class FullScreenQuad : public Geometry {

		RTTI_DECLARATIONS(FullScreenQuad, Geometry)

	public:
		FullScreenQuad(Game&);

		virtual void init() override;

	};

}