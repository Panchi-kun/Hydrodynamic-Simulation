#include <iostream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <vector>
using namespace std;
using namespace sf;

#pragma region Consts
static std::default_random_engine gen(std::random_device{}());
const unsigned int windowX = 1500;
const unsigned int windowY = 1000;
const float pi = 3.14159f;
#pragma endregion

struct Particle{
    float x, y, vx, vy, r;
    CircleShape ball;

    public:
    Particle(float r, Color color) : ball(r){
        ball.setFillColor(Color(color));
        
        uniform_real_distribution<double> rPos(0.0,min(windowX-2*r,windowY-2*r));
        uniform_real_distribution<double> rVel(0.0,0.2*windowX);
        x = rPos(gen); vx = rVel(gen);
        y = rPos(gen); vy = rVel(gen);
    }

    void set_pos(float inpX, float inpY){x=inpX; y=inpY;}
    void set_vel(float inpVx, float inpVy){vx=inpVx; vy=inpVy;}
    void reDraw(){ball.setPosition({x,y});}
};

vector<Particle> FlowParts(){
    float mRad = 10.0f;
    int num = 10;
    float linGap = windowY/num;
    
    vector<Particle> flows;
    for (int i=1; i-1<num; i++){
        Particle flowi(mRad, Color(35,137,218));
        flowi.set_pos(100,linGap*i);
        flowi.set_vel(70,0);

        flows.push_back(flowi);
    }

    return flows;
}

int main(){
    
    // Track time
    Clock clock;
    Time lastTime = clock.getElapsedTime();
    Time flowDraws = lastTime;


    // Define Particles
    vector <Particle> movers; 

    // Create & open display window
    RenderWindow window(VideoMode({windowX,windowY}),"Flow");
    window.setFramerateLimit(60);

    while (window.isOpen()){
        #pragma region Maintenance

        while (const optional event = window.pollEvent()){
            if (event->is<Event::Closed>())
                window.close();
        }
        
        window.display();
        window.clear(Color::Black);

        Time now = clock.getElapsedTime(); 
        float dt = (now - lastTime).asSeconds(); 
        lastTime = now; 

        // Draw all particles
        for (int i=0; i<movers.size(); i++){
            movers[i].reDraw();
            window.draw(movers[i].ball);
        }
        #pragma endregion

        // Update positions
        for (int i=0; i<movers.size(); i++){
            movers[i].x += movers[i].vx*dt; 
        }

        // Define new items
        int tGap = 1;
        cout <<now.asSeconds()<<endl;
        if ((now-flowDraws).asSeconds() < tGap){

            vector <Particle> flows = FlowParts();
            for (auto & flow : flows) {
                movers.push_back(flow);
                Time flowDraws = clock.getElapsedTime();
            }
        }
    }

    return 0;
}