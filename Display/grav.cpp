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
        ball.setPosition({x,y});
    }
};


void trajectory(Particle& par1, Particle& par2, float dt){

    float dx = par1.x-par2.x; float v1x = par1.v_x; float v2x = par2.v_x; 
    float dy = par1.y-par2.y; float v1y = par1.v_y; float v2y = par2.v_y;

    float theta = atan2(dy, dx); 
    float r     = sqrt(dx*dx + dy*dy);

    float a1 = -G*par2.mass/(r*r); 
    float a2 = G*par1.mass/(r*r);

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
    float v_orbital = sqrt(G*m1/R);


    vector<Particle> parts;
    parts.push_back(center);
    for (int i=0; i<detail; i++){
        float theta = 2*pi/detail*i;

        float px = x0 + R*cos(theta);
        float py = y0 + R*sin(theta);
        float vx = -v_orbital*sin(theta);
        float vy =  v_orbital*cos(theta);

        Particle ringi(2.0f, moonMass, sf::Color(230,230,230));
        ringi.set_pos(px, py);
        ringi.set_vel(vx, vy);
        parts.push_back(ringi);
    }

    return parts;
}


int main(){

    Particle s1(20.0f, earthMass, sf::Color(35,137,218)); s1.set_pos(600,200); s1.set_vel(0,0) ;
    int nRings = 30;
    int detail = 60;
    float startR = 50.0f;
    float gapR   = 8.0f;

    vector<Particle> parts = rings(s1, startR, detail);
    for (int k=1; k<nRings; k++){
        vector<Particle> ringk = rings(s1, startR + k*gapR, detail);
        parts.insert(parts.end(), ringk.begin()+1, ringk.end());
    }


    //Particle s2(20.0f, earthMass*.5, sf::Color(135,137,218)); s2.set_pos(1400,200); s2.set_vel(-180,80);
    //Particle s3(20.0f, earthMass*.5, sf::Color(135,137,218)); s3.set_pos(200,200);  s3.set_vel(180,20);
    Particle s2(20.0f, earthMass*.5, sf::Color(135,137,218)); s2.set_pos(1400,200); s2.set_vel(-80,80);
    Particle s3(20.0f, earthMass*.5, sf::Color(135,137,218)); s3.set_pos(200,900);  s3.set_vel(80,0);
    parts.push_back(s2);
    parts.push_back(s3);

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
            float ax = 0, ay = 0;
            for (int j=0; j<parts.size(); j++){
                if (i==j) continue;
                float dx = parts[j].x - parts[i].x;
                float dy = parts[j].y - parts[i].y;
                float r  = sqrt(dx*dx + dy*dy);
                float a  = G*parts[j].mass/(r*r);
                ax += a * dx/r;
                ay += a * dy/r;
            }
        parts[i].v_x += ax*dt;
        parts[i].v_y += ay*dt;
        }

        for (int i=0; i<parts.size(); i++){
        parts[i].x += parts[i].v_x*dt;
        parts[i].y += parts[i].v_y*dt;
        parts[i].ball.setPosition({parts[i].x-parts[i].radius, parts[i].y-parts[i].radius});
        }

        window.display();

    }

    return 0;
}