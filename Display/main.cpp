#include <SFML/Graphics.hpp>
#include <iostream>

float g = 9.8f;
unsigned int windowX = 800;
unsigned int windowY = 600;

float* velocity(float* curPos, float* inVel, sf::Time time) {
    float t = time.asSeconds();
    float v_x0 = inVel[0]; float v_y0 = inVel[1];
    
    float v_x = v_x0;
    float v_y = -g*t + v_y0;

    static float vel[2] = {v_x,v_y};
    return vel;
}

float* position(float* curPos, float* velo, sf::Time time) {
    float t = time.asSeconds();
    float x_0 = curPos[0]; float v_x = velo[0]; 
    float y_0 = curPos[1]; float v_y = velo[1];
    
    float x = v_x*t+x_0;
    float y = v_y*t+y_0;

    if (windowX <= x || x <= 0){
        v_x = -v_x;
    }
    if (windowY <= y || y <= 0){
        v_y = -v_y;
    }

    static float info[4] = {x,y,v_x,v_y};
    return info;
}


int main() {

    sf::RenderWindow window(sf::VideoMode({windowX,windowY}),"My Windows");
    window.setFramerateLimit(60);

    float vel[2] = {10.0f,20.0f};
    float pos[2] = {400.0f, 400.0f};

    sf::Clock clock;
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        
        
        // Plot the first ball, with initial conditions
        sf::CircleShape ball1(20.f);
        ball1.setFillColor(sf::Color(150,50,50));

        float* velo = velocity(pos, vel, clock.getElapsedTime());
        float* info = position(pos, velo, clock.getElapsedTime());
        pos[0] = info[0]; pos[1] = info[1];
        vel[0] = info[2]; vel[1] = info[2];

        ball1.setPosition({pos[0],pos[1]});
        window.draw(ball1);
        window.display();



    
    }

    return 0;
}