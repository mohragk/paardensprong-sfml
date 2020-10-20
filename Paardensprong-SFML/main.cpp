#include <SFML/graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>


struct Renderable {
    float x{ 0.0 }, y{ 0.0 };

    virtual void paint(sf::RenderWindow& window) = 0;
};

struct RenderableRectangle : Renderable {
    float w{ 100 }, h{ 100.0 };
    sf::Color color = sf::Color(sf::Color::Cyan);

    void paint(sf::RenderWindow& window) {
        sf::RectangleShape shape = sf::RectangleShape({ w, h });
        shape.setFillColor(color);
        shape.setPosition(x, y);
        window.draw(shape);
    }
};

struct RenderableText : Renderable {
    float text_size{ 30.0 };
    sf::String text = "";
    sf::Font font;
    sf::Color color = sf::Color(sf::Color::Black);

    void paint(sf::RenderWindow& window) override {
        sf::Text sf_text(text, font, text_size);
        sf_text.setFillColor(color);
        sf_text.setPosition({ x, y });
        window.draw(sf_text);
    }
};




namespace Renderer {

   
    std::vector<Renderable*> renderables = std::vector<Renderable*>();

    

    sf::Font getDefaultFont() {
        sf::Font default_font;
        default_font.loadFromFile("C:/dev/paardensprong-sfml/Paardensprong-SFML/x64/Debug/arial.ttf");

        return default_font;
    }
    
    inline void renderRectangle(const RenderableRectangle& rect, sf::RenderWindow& window) {
        
    }


    inline void renderText(const RenderableText& text, sf::RenderWindow& window) {
       
    }


    static void render(sf::RenderWindow &window) {
        window.clear();

        {
            // draw white background
            float w = window.getView().getSize().x;
            float h = window.getView().getSize().y;
            sf::RectangleShape bg = sf::RectangleShape({ w, h });
            bg.setFillColor(sf::Color(sf::Color::White));
            window.draw(bg);
        }

        for (Renderable *renderable : renderables) {
            renderable->paint(window);
        }

        renderables.clear();

        window.display();
    }

  

    static void addRectangle(RenderableRectangle *rect) {
        renderables.push_back(rect);
    }

    static void addText(RenderableText *text) {
        renderables.push_back(text);
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
    

    
    
    RenderableText test_text;

    test_text.text = "hello!";
    test_text.font = Renderer::getDefaultFont();
    test_text.text_size = 48;
    test_text.x = 120;
    test_text.y = 12;

   
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

        Renderer::addRectangle(&rect);
        Renderer::addText(&test_text);
        Renderer::render(window);
        
    }


    return 0;
}