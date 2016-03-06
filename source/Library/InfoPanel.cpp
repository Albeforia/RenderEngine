#include "InfoPanel.h"
#include <sstream>
#include <iomanip>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Game.h"
#include "Utility.h"

namespace Library {

	RTTI_DEFINITIONS(InfoPanel);

	InfoPanel::InfoPanel(Game& game)
		: GameComponentDrawable(game),
		m_sprite_batch {}, m_sprite_font {}, m_text_position {0.0f, 50.0f},
		m_frame_count {}, m_frame_rate {}, m_last_total_elapsed_time {} {}

	InfoPanel::~InfoPanel() {
		DeleteObject(m_sprite_font);
		DeleteObject(m_sprite_batch);
	}

	XMFLOAT2& InfoPanel::text_position() {
		return m_text_position;
	}

	int InfoPanel::frame_rate() const {
		return m_frame_rate;
	}

	void InfoPanel::init() {
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		m_sprite_batch = new SpriteBatch(m_game->d3d_device_context());
		m_sprite_font = new SpriteFont(m_game->d3d_device(), L"content\\fonts\\arial_14_r.spritefont");
	}

	void InfoPanel::update(const GameTime& game_time) {
		if (game_time.total_game_time() - m_last_total_elapsed_time >= 1) {
			m_last_total_elapsed_time = game_time.total_game_time();
			m_frame_rate = m_frame_count;
			m_frame_count = 0;
		}
		++m_frame_count;
	}

	void InfoPanel::draw(const GameTime& game_time) {
		m_sprite_batch->Begin();
		std::wostringstream fps;
		fps << std::setprecision(4) << L"FPS: " << m_frame_rate;
		m_sprite_font->DrawString(m_sprite_batch, fps.str().c_str(), m_text_position);
		m_sprite_batch->End();
	}

}