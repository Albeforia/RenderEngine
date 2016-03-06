#pragma once

#include "Common.h"

namespace Library {
	class ColorHelper {
	public:
		static const XMVECTORF32 Black;
		static const XMVECTORF32 White;
		static const XMVECTORF32 Red;
		static const XMVECTORF32 Green;
		static const XMVECTORF32 Blue;
		static const XMVECTORF32 Yellow;
		static const XMVECTORF32 BlueGreen;
		static const XMVECTORF32 Purple;
		static const XMVECTORF32 CornflowerBlue;
		static const XMVECTORF32 Wheat;
		static const XMVECTORF32 LightGray;

		ColorHelper() = delete;
		ColorHelper(const ColorHelper& rhs) = delete;
		ColorHelper& operator=(const ColorHelper& rhs) = delete;
	};
}