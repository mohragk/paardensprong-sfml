

#include "TextField.h"




void TextField::paint(sf::RenderWindow& window)
{
    // draw user input

    
    sf::Text text_shape = sf::Text(user_input, font, text_size);
    text_shape.setStyle(sf::Text::Bold);
    text_shape.setFillColor(text_color);

    if (disabled) {
        text_shape.setFillColor(sf::Color(22, 22, 22));
    }

    f32 center_x = x;
    f32 center_y = y;

    text_shape.setPosition(center_x, center_y);
    window.draw(text_shape);


    if (!disabled) {
        // draw cursor
        sf::RectangleShape cursor_rect = sf::RectangleShape({ 2, (f32)text_size });
        
        // animate cursor
        f32 color_mu = (std::sin(phase * 2.0 * PI) + 1.0) / 2.0;
       
        sf::Color cursor_color = util::colorLerp(sf::Color(sf::Color::White), sf::Color(sf::Color::Black), color_mu);
        cursor_rect.setFillColor(cursor_color);

        f32 cursor_pos_x = text_shape.findCharacterPos(cursor_index).x;
        cursor_rect.setPosition({ cursor_pos_x, y });
        window.draw(cursor_rect); 
    }

}

void TextField::keyPressed(sf::Event::KeyEvent& e)
{
    if (disabled) return;

    time_millis = 0.0f;

	std::string input = util::getStringFromKeyCode(e.code);
	if (input != "") {
        
		user_input.insert(cursor_index, input);
		cursor_index++;
	}
	else {
        if (e.code == sf::Keyboard::Left) {
            cursor_index--;
           
        }
        else if (e.code == sf::Keyboard::Right) {
            cursor_index++;
           
        }
        else if (e.code == sf::Keyboard::Backspace) {
            if (!user_input.size()) return;

            u16 erase_index = cursor_index - 1;
            if (erase_index >= 0) {
                user_input.erase(erase_index, 1);
                cursor_index--;
            }
        }
        else if (e.code == sf::Keyboard::Delete) {
            u16 erase_index = cursor_index;
            if (erase_index >= 0 && erase_index < user_input.size()) {
                user_input.erase(erase_index, 1);
            }
        }
        else if (e.code == sf::Keyboard::Enter) {
            for (TextFieldListener* listener : listeners) {
                listener->messageReceived(user_input);
            }
        }


        // clamp cursor
        if (cursor_index < 0) cursor_index = 0;
        if (cursor_index > user_input.size()) cursor_index = user_input.size();
	}

	
}

void TextField::reset()
{
    cursor_index = 0;
    user_input = "";
}
