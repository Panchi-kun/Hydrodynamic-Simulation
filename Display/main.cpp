#include <SFML/Graphics.hpp>
#include <iostream>

unsigned int windowX = 800;
unsigned int windowY = 600;

float g = 9.8f;
float rad1 = 20.f;



int main() {

    sf::RenderWindow window(sf::VideoMode({windowX,windowY}),"My Windows");
    window.setFramerateLimit(60);

    float vel[2] = {200.0f,200.0f};
    float pos[2] = {400.0f, 400.0f};

    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Time now = clock.getElapsedTime();
        float dt = (now - lastTime).asSeconds();
        lastTime = now;

        vel[1] += g*dt;
        pos[0] += vel[0]*dt;
        pos[1] += vel[1]*dt;

        if (0 >= pos[0] || pos[0]+2*rad1 >= windowX) vel[0] = -vel[0];
        if (0 >= pos[1] || pos[1]+2*rad1 >= windowY) vel[1] = -vel[1];

        window.clear(sf::Color::Black);
        
        
        // Plot the first ball, with initial conditions
        sf::CircleShape ball1(rad1);
        ball1.setFillColor(sf::Color(150,50,50));

        ball1.setPosition({pos[0],pos[1]});
        window.draw(ball1);
        window.display();



    
    }

    return 0;
}