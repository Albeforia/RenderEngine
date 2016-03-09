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
		Material();
		Material(const std::string& default_techname);
		virtual ~Material();

		Variable* operator[](const std::string& name);
		Effect* effect() const;
		Technique* curr_technique() const;
		void set_curr_technique(Technique*);
		const std::map<Pass*, ID3D11InputLayout*>& input_layouts() const;

		virtual void init(Effect*);
		virtual UINT vertex_size() const = 0;
		virtual void create_vertex_buffer(ID3D11Device*, const Model&, std::vector<ID3D11Buffer*>&) const;
		virtual void create_vertex_buffer(ID3D11Device*, const Mesh&, ID3D11Buffer**) const = 0;

	protected:
		virtual void create_input_layout(const std::string& techname, const std::string& passname, D3D11_INPUT_ELEMENT_DESC*, UINT num);
		virtual void create_vertex_buffer(ID3D11Device*, void* vertices, UINT count, ID3D11Buffer**) const;

		Effect* m_effect;
		Technique* m_curr_technique;
		std::string m_default_techname;
		std::map<Pass*, ID3D11InputLayout*> m_input_layouts;
	};

}