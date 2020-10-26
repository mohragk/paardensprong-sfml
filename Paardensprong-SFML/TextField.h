#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>

#include "util.h"

class TextFieldListener {
public:
	virtual ~TextFieldListener() {};
	virtual void actionPerformed(const std::string& message) = 0;
};

struct TextField {
	std::vector<TextFieldListener *> listeners = std::vector<TextFieldListener *>();
	f32 x{ 100.0f }, y{ 100.0f };
	u16 max_input_length{ 20 };

	u32 text_size{ 28 };
	sf::Font font{ util::getDefaultFont() };
	sf::Color text_color{ util::getTextColor() };
	sf::Color selection_color{ util::getInputSelectionColor() };

	f64 phase = 0.0;
	f32 time_millis{ 0.0f };
	f32 blink_duration_millis{ 1000.0f };

	std::string user_input{""};
	i16 cursor_index{ 0 };
	i16 selection_cursor_index{ 0 };

	bool disabled{ false };
	bool selecting{ false };
	bool in_focus{ true };

	void position(f32 new_x, f32 new_y) {
		x = new_x;
		y = new_y;
	}

	void update(f32 dt) {

		time_millis += dt;
		
		if (time_millis >= blink_duration_millis) {
			time_millis = 0.0f;
		}
		
		phase = time_millis / blink_duration_millis;
		phase = phase - (i32)phase; //might be unnecessay 
	}

	void resizeText(u32 text_size) {
		this->text_size = text_size;
	}

	void paint(sf::RenderWindow& window);

	void keyPressed(sf::Event::KeyEvent& e);

	void reset();

	void disable(bool should) {
		disabled = should;
	}

	void addListener(TextFieldListener* listener) {
		listeners.push_back(listener);
	}

};