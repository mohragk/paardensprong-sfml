

#include "TextField.h"


void TextField::paint(sf::RenderWindow& window)
{
   

    // draw user input
   
    sf::Text text_shape = sf::Text(user_input, font, text_size);
    text_shape.setFillColor(text_color);
    if (disabled) {
        text_shape.setFillColor(util::getInputDisabledTextColor());
    }

    text_shape.setPosition(x, y);




    //draw selection background
    f32 selection_x = text_shape.findCharacterPos(cursor_index).x;
    f32 selection_y = y + 4;
    f32 selection_x_max = text_shape.findCharacterPos(selection_cursor_index).x;
    f32 selection_width = selection_x_max - selection_x;

    sf::RectangleShape selection_shape = sf::RectangleShape({ selection_width, (f32)text_size });
    selection_shape.setFillColor( selection_color );
    selection_shape.setPosition({ selection_x, selection_y });
    
    
    window.draw(selection_shape);


    // draw text over the selection shape

    u16 valid_user_input_size = 8;
    if (user_input.size() > valid_user_input_size) {
        std::string valid_user_input = user_input.substr(0, valid_user_input_size);
        std::string extraneous_user_input = user_input.substr(valid_user_input_size, user_input.size() - 1);
        
        sf::Text valid_text_shape = sf::Text(valid_user_input, font, text_size);
        valid_text_shape.setPosition(text_shape.getPosition());
        valid_text_shape.setFillColor(text_shape.getFillColor());

        f32 extraneous_user_input_x = valid_text_shape.findCharacterPos(valid_user_input.size()).x;
        sf::Text extraneous_text_shape = sf::Text(extraneous_user_input, font, text_size);
        extraneous_text_shape.setPosition(extraneous_user_input_x, valid_text_shape.getPosition().y);
        extraneous_text_shape.setFillColor(util::getInvalidTextColor());

        window.draw(valid_text_shape);
        window.draw(extraneous_text_shape);
    }
    else {
        window.draw(text_shape);
    }


    if (!disabled && in_focus) {
        // draw cursor
        sf::RectangleShape cursor_rect = sf::RectangleShape({ 2.0f, (f32)text_size });
        
        // animate cursor
        f32 color_mu = f32( ( std::sin(phase * 2.0 * PI) + 1.0) / 2.0 );
       
        sf::Color cursor_color = util::colorLerp(util::getStandardBackgroundColor(), util::getTextColor(), color_mu);
        cursor_rect.setFillColor(cursor_color);

        f32 cursor_pos_x = text_shape.findCharacterPos(cursor_index).x;
        f32 cursor_pos_y = y + 4;
        cursor_rect.setPosition({ cursor_pos_x, cursor_pos_y });
        window.draw(cursor_rect); 
    }


    
}

void TextField::keyPressed(sf::Event::KeyEvent& e)
{
    if (disabled) return;

    

    time_millis = 0.0f;
    
	std::string character_input = util::getStringFromKeyCode(e.code);
	if (character_input != "") {
        
        if (user_input.size() < max_input_length) {
            user_input.insert(cursor_index, character_input);
            cursor_index++;
        }
		
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

            if (!selecting) {
                i16 erase_index = cursor_index - 1;
                if (erase_index >= 0) {
                    user_input.erase(erase_index, 1);
                    cursor_index--;
                }
            }
            else {
                i16 erase_index = std::min(cursor_index, selection_cursor_index);
                i16 erase_count = std::abs(selection_cursor_index - cursor_index);
                user_input.erase(erase_index, erase_count);
                cursor_index = erase_index; 
                selection_cursor_index = cursor_index;
            }

            
        }
        else if (e.code == sf::Keyboard::Delete) {
            if (!user_input.size()) return;
            if (!selecting) {
                i16 erase_index = cursor_index;
                if (erase_index >= 0 && erase_index < user_input.size()) {
                    user_input.erase(erase_index, 1);
                }
            }
            else {
                i16 erase_index = std::min(cursor_index, selection_cursor_index);
                i16 erase_count = std::abs(selection_cursor_index - cursor_index);
                user_input.erase(erase_index, erase_count);
                cursor_index = erase_index;
                selection_cursor_index = cursor_index;
            }
            
        }
        else if (e.code == sf::Keyboard::Home) {
            cursor_index = 0;
        }
        else if (e.code == sf::Keyboard::End) {
            cursor_index = (i16) user_input.size();
        }
        else if (e.code == sf::Keyboard::Enter) {
            for (TextFieldListener* listener : listeners) {
                listener->actionPerformed(user_input);
            }
        }


        if (e.control) {
            if (e.code == sf::Keyboard::Left) {
                cursor_index = 0;
            }
            else if (e.code == sf::Keyboard::Right) {
                cursor_index = (i16) user_input.size();
            }
        }
        
	}

    // clamp cursor
    if (cursor_index < 0) cursor_index = 0;
    if (cursor_index > user_input.size()) cursor_index = i16(user_input.size());

    if (!e.shift) {
        selection_cursor_index = cursor_index;
        selecting = false;
    }
    else {
        if (cursor_index != selection_cursor_index) selecting = true;
        else selecting = false;
    }
}

void TextField::reset()
{
    cursor_index = 0;
    user_input = "";
}
