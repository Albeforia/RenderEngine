#pragma once

#include "GameComponentDrawable.h"

namespace DirectX {

	class SpriteBatch;
	class SpriteFont;

}

namespace Library {

	class InfoPanel : public GameComponentDrawable {

		RTTI_DECLARATIONS(InfoPanel, GameComponentDrawable);

	public:

		InfoPanel(Game&);
		~InfoPanel();
		InfoPanel(const InfoPanel&) = delete;
		InfoPanel& operator=(const InfoPanel&) = delete;

		XMFLOAT2& text_position();
		int frame_rate() const;

		virtual void init() override;
		virtual void update(const GameTime&) override;
		virtual void draw(const GameTime&) override;

	private:

		SpriteBatch* m_sprite_batch;
		SpriteFont* m_sprite_font;
		XMFLOAT2 m_text_position;

		int m_frame_count;
		int m_frame_rate;
		double m_last_total_elapsed_time;

	};

}