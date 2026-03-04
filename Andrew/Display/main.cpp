#include <SFML/Graphics.hpp>

int main() {
    // SFML 3 uses Vector2u for window size
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "SFML in VSCode");
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    
    while (window.isOpen()) {
        // SFML 3 uses std::optional for event polling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        window.clear();
        window.draw(shape);
        window.display();
    }
    
    return 0;
}