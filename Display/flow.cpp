#include <iostream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <vector>
using namespace std;
using namespace sf;

#pragma region Constants
static std::default_random_engine gen(std::random_device{}());
const unsigned int windowX = 1500;
const unsigned int windowY = 1000;
const float pi = 3.14159f;
const int tGap = 1;
#pragma endregion

// Create displaceable particles
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

// Create background objects
struct stillObject{
    float x; float xSz;
    float y; float ySz; 
    unique_ptr<Shape> obj;

    void makeRect(){
        auto rect = make_unique<RectangleShape>();
        rect->setSize({xSz, ySz});
        obj = move(rect);
    }
    void makeCirc(){
        auto cir = make_unique<CircleShape>(xSz);
        obj = move(cir);
    }
    void makeTri(){
        auto tri = make_unique<CircleShape>(xSz,3);
        obj = move(tri);
    }

    public:
    stillObject(vector<float> pos, vector<float> shape, Color color, char creed){
        x=pos[0]; xSz=shape[0];
        y=pos[1]; ySz=shape[1];

        if      (creed == 'r'){makeRect();}
        else if (creed == 'c'){makeCirc();}
        else if (creed == 't'){makeTri();}

        obj->setFillColor(Color(color));
        reDraw();
        
    }
    void reDraw(){obj->setPosition({x,y});}
    void setRotation(float ang){
        obj->rotate(obj->getRotation()+degrees(ang));
    }
};

// Create default uniform flow particles
vector<Particle> flowParts(){
    float mRad = 10.0f;
    float iniPos = 50.0f;
    float iniVx = 100.0f;
    float iniVy = 0.0f;

    int num = 10;
    float linGap = windowY/num;
    
    vector<Particle> flows;
    for (int i=1; i-1<num; i++){
        Particle flowi(mRad, Color(35,137,218));
        flowi.set_pos(iniPos,linGap*i);
        flowi.set_vel(iniVx,iniVy);

        flows.push_back(flowi);
    }

    return flows;
}

int main(){
    
    #pragma region Junk

    // Track time
    Clock clock;
    Time lastTime = clock.getElapsedTime();
    Time lastFlow = lastTime;

    // Define Particles
    vector <Particle> movers; 

    // Create & open display window
    RenderWindow window(VideoMode({windowX,windowY}),"Flow");
    window.setFramerateLimit(60);

    #pragma endregion
    #pragma region CreateCar

    vector<stillObject> objs;
    stillObject lBody({700.f,400.f},{400.f,75.f},Color(137,137,218),'r');
    stillObject hBody({800.f,335.f},{300.f,70.f},Color(137,137,218),'r');

    stillObject wheel1({720.f,450.f},{40.f,0.f},Color(218,137,218),'c');
    stillObject wheel2({980.f,450.f},{40.f,0.f},Color(218,137,218),'c');

    stillObject noseCone({662.3f,378.f},{43.f,0.f},Color(137,137,218),'t');
    noseCone.setRotation(30);

    objs.push_back(move(lBody));
    objs.push_back(move(hBody));
    objs.push_back(move(wheel1));
    objs.push_back(move(wheel2));
    objs.push_back(move(noseCone));

    #pragma endregion

    while (window.isOpen()){
        #pragma region WindowTimeMaintance

        while (const optional event = window.pollEvent()){
            if (event->is<Event::Closed>())
                window.close();
        }
        
        window.display();
        window.clear(Color::Black);

        Time now = clock.getElapsedTime(); 
        float dt = (now - lastTime).asSeconds(); 
        lastTime = now; 

        #pragma endregion

        // Draw all particles
        for (int i=0; i<movers.size(); i++){
            movers[i].reDraw();
            window.draw(movers[i].ball);
        }
        // Draw all shapes
        for (int i=0; i<objs.size(); i++){
            objs[i].reDraw();
            window.draw(*objs[i].obj.get());
        }      

        // Update positions
        for (int i=0; i<movers.size(); i++){
            if (movers[i].x< 0 || windowX<movers[i].x || movers[i].y<0 || windowY<movers[i].y){
                movers.erase(movers.begin() + i);
                continue;
            }
            movers[i].x += movers[i].vx*dt; 
            movers[i].y += movers[i].vy*dt; 
        }
        // cout << movers.size() <<endl; // Make sure particle sizes arent exploding

        // Define flow particles
        if ((now-lastFlow).asSeconds() > tGap){
            vector <Particle> flows = flowParts();
            for (auto & flow : flows) {
                movers.push_back(flow);
            }
            lastFlow = lastTime;
        }        
    }

    return 0;
}