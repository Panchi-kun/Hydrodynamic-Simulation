#include <iostream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

const unsigned int windowX = 1500;
const unsigned int windowY = 1000;

const float pi = 3.14159f;
//const float G = 6.674e-11f;
//const float earthMass = 5.672e24f;
//const float moonMass  = 7.246e22f;

const float G        = 1.0f;
const float earthMass = 1e6f;  
const float moonMass  = 1.0f;   

static std::default_random_engine gen(std::random_device{}());


struct Particle{
    float x, y, v_x, v_y, radius, mass;
    sf::CircleShape ball;

  public:
    Particle(float r, float m, sf::Color color) : ball(r), radius(r), mass(m){
        ball.setFillColor(sf::Color(color));
        
        std::uniform_real_distribution<double> rPos(0.0,min(windowX-2*radius,windowY-2*radius));
        std::uniform_real_distribution<double> rVel(0.0,0.2*windowX);
        x = rPos(gen); v_x = rVel(gen);
        y = rPos(gen); v_y = rVel(gen);
    }

    void set_pos(float inpX, float inpY){
        x=inpX; y=inpY;
    }
    void set_vel(float inpVx, float inpVy){
        v_x=inpVx; v_y=inpVy;
    }

    void reDraw(){
        ball.setPosition({x-radius,y-radius});
    }
};


void trajectory(Particle& par1, Particle& par2, float dt){

    float dx = par1.x-par2.x; float v1x = par1.v_x; float v2x = par2.v_x; 
    float dy = par1.y-par2.y; float v1y = par1.v_y; float v2y = par2.v_y;

    float theta = atan2(dy, dx); 
    float r     = sqrt(dx*dx + dy*dy);

    float a1 = -G*par2.mass/(r*r); 
    float a2 =  G*par1.mass/(r*r);

    // Find updated velocities
    par1.v_x += a1*dt*cos(theta); par2.v_x += a2*dt*cos(theta);
    par1.v_y += a1*dt*sin(theta); par2.v_y += a2*dt*sin(theta);

    // Update positions
    par1.x += par1.v_x*dt; par2.x += par2.v_x*dt;
    par1.y += par1.v_y*dt; par2.y += par2.v_y*dt;

    par1.ball.setPosition({par1.x,par1.y});
    par2.ball.setPosition({par2.x,par2.y});


}

vector<Particle> rings(Particle center, float R, int detail){
    float m1 = center.mass;
    float x0 = center.x;
    float y0 = center.y;

    vector<float> drawXs(detail), drawVxs(detail);
    vector<float> drawYs(detail), drawVys(detail);
    for (int i=0; i<detail; i++){
        float theta = 2*pi/detail*i;
        float v_orbital = sqrt(G*m1/R);

        drawXs[i] = x0+R*cos(theta); drawVxs[i] = -v_orbital*sin(theta);
        drawYs[i] = y0+R*sin(theta); drawVys[i] =  v_orbital*cos(theta);
    } 


    vector<Particle> parts;
    parts.push_back(center);
    for (int i=0; i<detail; i++){
        Particle ringi(8.0f, moonMass, sf::Color(230,230,230)) ; ringi.set_pos(drawXs[i],drawYs[i]); ringi.set_vel(drawVxs[i],drawVys[i]);
        parts.push_back(ringi);
    }
    

    return parts;
}


// Window is 800x800 px^2

int main(){

    Particle s1(20.0f, earthMass, sf::Color(35,137,218)); s1.set_pos(600,400); s1.set_vel(0,0) ;
    vector<Particle> parts = rings(s1,200,2);


    //Particle s2(8.0f, moonMass *pxtometer, sf::Color(230,230,230)) ; s2.set_pos(100,400); s2.set_vel(0,-100);
    //Particle s3(8.0f, moonMass *pxtometer, sf::Color(230,230,230)) ; s3.set_pos(700,400); s3.set_vel(0,100);
    //vector<Particle> parts = {s1, s2,s3};
    
    //Particle s2(8.0f, moonMass *pxtometer, sf::Color(10,50,150)) ; s2.set_pos(100,400); s2.set_vel(0,0);
    //Particle s3(8.0f, moonMass *pxtometer, sf::Color(10,150,150)); s3.set_pos(700,400); s3.set_vel(0,0);
    
    //Particle s4(8.0f, moonMass *pxtometer, sf::Color(100,50,150)) ; s4.set_pos(400,100); s4.set_vel(0,0);
    //Particle s5(8.0f, moonMass *pxtometer, sf::Color(100,150,150)); s5.set_pos(400,700); s5.set_vel(0,0);
    
    //vector<Particle> parts = {s1, s2, s3,s4,s5};


    sf::RenderWindow window(sf::VideoMode({windowX,windowY}),"My Windows");
    window.setFramerateLimit(60);

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
        
        for (int i=0; i<parts.size(); i++){
            window.draw(parts[i].ball);
        }

        for (int i=0; i<parts.size(); i++){
            for (int j=0; j<parts.size(); j++){
                if (i==j || j>i) continue;

                trajectory(parts[i],parts[j],dt);
            }
        }

        window.display();

    }

    return 0;
}