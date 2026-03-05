#include <SFML/Graphics.hpp>
#include <iostream>

float* position(sf::Time time) {
    float x_0  = 100.0f; float y_0  = 300.0f;
    float v_x0 = 50.0f ; float v_y0 = -50.0f;
    float g = 9.8f; 
    
    float sec = time.asSeconds();

    float x = v_x0*sec + x_0;
    float y = 0.5*g*sec*sec + v_y0*sec + y_0;

    static float pos[2];
    pos[0] = x;
    pos[1] = y;
    return pos;
}


int main() {

    sf::RenderWindow window(sf::VideoMode({800,600}),"My Windows");
    window.setFramerateLimit(60);

    sf::Clock clock;
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        
        sf::CircleShape shape(20.f);
        shape.setFillColor(sf::Color(150,50,50));

        static float pos[2];
        float* vals = position(clock.getElapsedTime()); 
        shape.setPosition({vals[0],vals[1]});

        window.draw(shape);
        window.display();

    }

    return 0;
}