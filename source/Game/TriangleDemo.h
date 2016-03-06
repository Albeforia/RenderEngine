#pragma once

#include <d3dx11effect.h>
#include "GameComponentDrawable.h"

using namespace Library;

namespace Rendering {

	class TriangleDemo : public GameComponentDrawable {

		RTTI_DECLARATIONS(TriangleDemo, GameComponentDrawable);

	public:
		TriangleDemo(Game&, Camera&);
		~TriangleDemo();

		TriangleDemo() = delete;
		TriangleDemo(const TriangleDemo&) = delete;
		TriangleDemo& operator=(const TriangleDemo&) = delete;

		virtual void init() override;
		virtual void draw(const GameTime&) override;

	private:
		typedef struct _BasicEffectVertex {
			XMFLOAT4 position;
			XMFLOAT4 color;

			_BasicEffectVertex() {}
			_BasicEffectVertex(XMFLOAT4 p, XMFLOAT4 c)
				: position {p}, color {c} {}
		} BasicEffectVertex;

		ID3DX11Effect* m_effect;
		ID3DX11EffectTechnique* m_technique;
		ID3DX11EffectPass* m_pass;
		ID3DX11EffectMatrixVariable* m_wvp;
		ID3D11InputLayout* m_input_layout;
		ID3D11Buffer* m_vertex_buffer;

		XMFLOAT4X4 m_world_mat;

	};

}