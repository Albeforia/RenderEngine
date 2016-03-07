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

	Effect&Technique::effect() { return m_effect; }
	ID3DX11EffectTechnique*Technique::technique() const { return m_technique; }
	const D3DX11_TECHNIQUE_DESC&Technique::technique_desc() const { return m_technique_desc; }
	const std::string&Technique::name() const { return m_name; }
	const std::vector<Pass*>&Technique::passes() const { return  m_passes; }
	const std::map<std::string, Pass*>&Technique::passes_by_name() const { return m_passes_by_name; }

}