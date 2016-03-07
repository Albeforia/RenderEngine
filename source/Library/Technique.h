#pragma once

#include "Common.h"
#include "Pass.h"

namespace Library {

	class Game;
	class Effect;

	class Technique {

	public:
		Technique(Game&, Effect&, ID3DX11EffectTechnique*);

		Technique(const Technique&) = delete;
		Technique& operator=(const Technique&) = delete;

		Effect& effect();
		ID3DX11EffectTechnique* technique() const;
		const D3DX11_TECHNIQUE_DESC& technique_desc() const;
		const std::string& name() const;
		const std::vector<Pass*>& passes() const;
		const std::map<std::string, Pass*>& passes_by_name() const;

	private:
		Effect& m_effect;
		ID3DX11EffectTechnique* m_technique;
		D3DX11_TECHNIQUE_DESC m_technique_desc;
		std::string& m_name;
		std::vector<Pass*> m_passes;
		std::map<std::string, Pass*> m_passes_by_name;

	};

}