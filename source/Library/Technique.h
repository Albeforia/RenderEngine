#pragma once

#include "Common.h"
#include "Pass.h"

namespace Library {

	class Game;
	class Effect;

	/**
	 *  Wrapper around ID3DX11EffectTechnique.
	 *  Technique owns Pass.
	 */
	class Technique {

	public:
		Technique(Game&, Effect&, ID3DX11EffectTechnique*);
		~Technique();

		const std::string& name() const;

		/**
		*  Get pass by index.
		*/
		const Pass* pass(int) const;
		/**
		*  Get pass by name.
		*/
		const Pass* pass(const std::string&) const;

	private:
		Effect& m_effect;

		ID3DX11EffectTechnique* m_technique;
		D3DX11_TECHNIQUE_DESC m_technique_desc;

		std::string m_name;
		std::vector<Pass*> m_passes;
		std::map<std::string, Pass*> m_passes_by_name;

	};

}