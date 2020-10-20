#include <SFML/graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>


struct RenderableRectangle {
    float x{ 0.0 }, y{ 0.0 };
    float w{ 100 }, h{ 100.0 };
    sf::Color color = sf::Color(sf::Color::Cyan);
};

static void renderRectangle(const RenderableRectangle& rect, sf::RenderWindow& window) {
    sf::RectangleShape shape = sf::RectangleShape({ rect.w, rect.h });
    shape.setFillColor(rect.color);
    shape.setPosition(rect.x, rect.y);
    window.draw(shape);
}


class Renderer {
public:
    std::vector<RenderableRectangle> rects;

    sf::Font default_font;
    sf::Text test_text;

    Renderer() {


        if (!default_font.loadFromFile("arial.ttf")) {
            std::cout << "Loading font failed!" << "\n";
        }
        // Create a text
        test_text = sf::Text("hello", default_font);
        test_text.setCharacterSize(30);
        test_text.setStyle(sf::Text::Bold);
        test_text.setFillColor(sf::Color::Red);
    }
    

    void render(sf::RenderWindow &window) {
        for (RenderableRectangle& rect : rects) {
            renderRectangle(rect, window);
        }


       
       
        window.draw(test_text);
    }

    void addRectangle(RenderableRectangle rect) {
        rects.push_back(rect);
    }
};






int main()
{
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Paardensprong Game", sf::Style::Default, settings );
    
    // activate the window
    window.setActive(true);
    window.setFramerateLimit(60);


    RenderableRectangle rect;
    rect.x = 12;
    rect.y = 24;
    rect.w = 240;
    rect.h = 180;
    

    Renderer renderer;
    renderer.addRectangle(rect);
    

    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
               
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                // Reset view
                sf::View view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);
                
            }
                
        }

        window.clear();

        {

            float w = window.getView().getSize().x;
            float h = window.getView().getSize().y;
            sf::RectangleShape bg = sf::RectangleShape({w, h});
            bg.setFillColor(sf::Color(sf::Color::White));
            window.draw(bg);
        }

        renderer.render(window);

        window.display();
        
    }


    return 0;
}