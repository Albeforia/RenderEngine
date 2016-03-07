#include "Material.h"
#include "GameException.h"
#include "Model.h"

namespace Library {

	RTTI_DEFINITIONS(Material);

	Material::Material()
		: m_effect {}, m_curr_technique {}, m_default_techname {}, m_input_layouts {} {}

	Material::Material(const std::string& default_techname)
		: m_effect {}, m_curr_technique {}, m_default_techname {default_techname}, m_input_layouts {} {}

	Material::~Material() {
		for (auto pass_layout : m_input_layouts) {
			ReleaseObject(pass_layout.second);
		}
	}

	Variable* Material::operator[](const std::string& name) {
		auto& variables = m_effect->variables_by_name();
		Variable* v = nullptr;
		auto it = variables.find(name);
		if (it != variables.end()) {
			v = it->second;
		}
		return v;
	}

	void Material::set_curr_technique(Technique* t) {
		m_curr_technique = t;
	}

	void Material::init(Effect* effect) {
		m_effect = effect;
		assert(m_effect != nullptr);
		Technique* default_tech = nullptr;
		assert(m_effect->techniques().size() > 0);
		if (!m_default_techname.empty()) {
			default_tech = m_effect->techniques_by_name().at(m_default_techname);
			assert(default_tech != nullptr);
		}
		else {
			default_tech = m_effect->techniques().at(0);
		}
		set_curr_technique(default_tech);
	}

	void Material::create_vertex_buffer(ID3D11Device* dev, const Model& model, std::vector<ID3D11Buffer*>& buffers) const {
		buffers.reserve(model.meshes().size());
		for (auto* mesh : model.meshes()) {
			ID3D11Buffer* buffer;
			create_vertex_buffer(dev, *mesh, &buffer);
			buffers.push_back(buffer);
		}
	}

	void Material::create_input_layout(const std::string& techname, const std::string& passname, D3D11_INPUT_ELEMENT_DESC* desc, UINT num) {
		Technique* tech = m_effect->techniques_by_name().at(techname);
		assert(tech != nullptr);
		Pass* pass = tech->passes_by_name().at(passname);
		assert(pass != nullptr);
		ID3D11InputLayout* layout;
		pass->create_input_layout(desc, num, &layout);
		m_input_layouts.insert({pass, layout});
	}

}