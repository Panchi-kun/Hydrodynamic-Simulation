#include <SFML/Graphics.hpp>
#include <iostream>

unsigned int windowX = 800;
unsigned int windowY = 600;

float g = 90.8f;
float rad1 = 20.f;

float* ballLocational(float dt, float* info){
    info[3] += g*dt;
    info[0] += info[2]*dt;
    info[1] += info[3]*dt;
    
    return info;
}

float* wallCollision(float* info){
    if (info[0] <= 0 || info[0]+2*rad1 >= windowX) info[2] = -info[2];
    if (info[1] <= 0 || info[1]+2*rad1 >= windowY) info[3] = -info[3];
    return info;
}

int main() {

    sf::RenderWindow window(sf::VideoMode({windowX,windowY}),"My Windows");
    window.setFramerateLimit(60);

    float info1[4] = {200.0f,200.0f,100.0f, 100.0f};
    float info2[4] = {100.0f,400.0f,100.0f, 100.0f};

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

        window.clear(sf::Color::Black);
        
        
        // Plot the first ball, with initial conditions
        sf::CircleShape ball1(rad1); ball1.setFillColor(sf::Color(150,50,50));

        ballLocational(dt,info1);
        wallCollision(info1);
        ball1.setPosition({info1[0],info1[1]});
        
        window.draw(ball1);
        window.display();

        // Plot the second ball, with initial conditions
        sf::CircleShape ball2(rad1); ball2.setFillColor(sf::Color(50,50,150));

        ballLocational(dt,info2);
        wallCollision(info2);
        ball2.setPosition({info2[0],info2[1]});
        
        window.draw(ball2);
        window.display();
    
    }

    return 0;
}