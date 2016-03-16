#pragma once

#include "Common.h"
#include "Technique.h"
#include "Variable.h"

namespace Library {

	class Game;

	/**
	 *  Wrapper around ID3DX11Effect.
	 *  Effect owns Technique and Variable.
	 */
	class Effect {
	public:
		Effect(Game&);
		virtual ~Effect();

		static void compile_from_file(ID3D11Device*, ID3DX11Effect**, const std::wstring& file);
		static void load(ID3D11Device*, ID3DX11Effect**, const std::wstring& file);

		void compile_from_file(const std::wstring& file);
		void load(const std::wstring& file);

		/**
		 *  Get technique by index.
		 */
		const Technique* technique(int) const;
		/**
		 *  Get technique by name.
		 */
		const Technique* technique(const std::string&) const;
		/**
		*  Get variable by index.
		*/
		Variable* variable(int) const;
		/**
		*  Get variable by name.
		*/
		Variable* variable(const std::string&) const;

		const Game& game() const;

	private:
		void init();

		Game& m_game;
		ID3DX11Effect* m_effect;
		D3DX11_EFFECT_DESC m_effect_desc;

		std::vector<Technique*> m_techniques;
		std::map<std::string, Technique*> m_techniques_by_name;
		std::vector<Variable*> m_variables;
		std::map<std::string, Variable*> m_variables_by_name;

	};

}