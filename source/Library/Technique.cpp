#include "Technique.h"

namespace Library {

	Technique::Technique(Game& game, Effect& effect, ID3DX11EffectTechnique* technique)
		: m_effect {effect}, m_technique {technique}, m_technique_desc {},
		m_name {}, m_passes {}, m_passes_by_name {} {
		m_technique->GetDesc(&m_technique_desc);
		m_name = m_technique_desc.Name;
		for (UINT i = 0; i < m_technique_desc.Passes; i++) {
			auto* p = new Pass(game, *this, m_technique->GetPassByIndex(i));
			m_passes.push_back(p);
			m_passes_by_name.insert({p->name(), p});
		}
	}

	Technique::~Technique() {
		for (auto* p : m_passes) {
			DeleteObject(p);
		}
		ReleaseObject(m_technique);
	}

	const std::string& Technique::name() const { return m_name; }

	const Pass* Technique::pass(int index) const {
		return m_passes.at(index);
	}

	const Pass* Technique::pass(const std::string& name) const {
		return m_passes_by_name.at(name);
	}

}