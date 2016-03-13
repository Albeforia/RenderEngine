#pragma once

#include "Common.h"
#include "Effect.h"
#include "EffectHelper.h"

namespace Library {

	class Model;
	class Mesh;

	class Material : public RTTI {

		RTTI_DECLARATIONS(Material, RTTI);

	public:
		virtual ~Material();

		const Effect* effect() const;

		const Technique* curr_technique() const;
		/**
		 *  Set current technique by index.
		 */
		void set_curr_technique(int);
		/**
		 *  Set current technique by name.
		 */
		void set_curr_technique(const std::string&);

		void apply(ID3D11DeviceContext*) const;

		virtual void init(Effect*);
		virtual UINT vertex_size() const = 0;
		virtual void create_vertex_buffer(ID3D11Device*, const Model&, std::vector<ID3D11Buffer*>&) const;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const = 0;

		void create_buffer(ID3D11Device*, void* data, UINT count, UINT size,
						   D3D11_USAGE, ID3D11Buffer**) const;

	protected:
		const Effect* m_effect;
		const Technique* m_curr_technique;

	};

}