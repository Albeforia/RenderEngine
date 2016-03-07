#pragma once

#include "Common.h"
#include "Technique.h"
#include "Variable.h"

namespace Library {

	class Game;

	class Effect {

	public:
		Effect(Game&);
		virtual ~Effect();

		Effect(const Effect&) = delete;
		Effect& operator=(const Effect&) = delete;

		static void compile_from_file(ID3D11Device*, ID3DX11Effect**, const std::wstring& file);
		static void load(ID3D11Device*, ID3DX11Effect**, const std::wstring& file);

		Game& game();
		ID3DX11Effect* effect() const;
		void set_effect(ID3DX11Effect*);
		const D3DX11_EFFECT_DESC& effect_desc() const;
		const std::vector<Technique*>& techniques() const;
		const std::map<std::string, Technique*>& techniques_by_name() const;
		const std::vector<Variable*>& variables() const;
		const std::map<std::string, Variable*>& variables_by_name() const;

		void compile_from_file(const std::wstring& file);
		void load(const std::wstring& file);

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