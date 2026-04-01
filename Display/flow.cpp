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
const float G = 10000.0f;
const int px = 10;
const float objMass = 5.f;
#pragma endregion

// Create displaceable particles
struct Particle{
    float x, y, vx, vy, r, m;
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
    float m = 10.0f;
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

void removeBounds(vector<Particle>& parts){
    parts.erase(
        remove_if(parts.begin(), parts.end(), [](const Particle& p){
            return p.x < 0 || p.x > windowX || p.y < 0 || p.y > windowY;
        }),
        parts.end()
    );
    parts.erase(
        remove_if(parts.begin(), parts.end(), [](const Particle& p){
            return p.vx < -40;
        }),
        parts.end()
    );
}

// Create default uniform flow
vector<Particle> flowParts(){
    float mRad = 1.5f;
    float mass = 10.f;
    float iniPos = 200.f;
    float iniVx = 100.f;
    float iniVy = 0.f;

    int num = 50;
    float linGap = windowY/num;
    
    vector<Particle> flows;
    for (int i=1; i-1<num; i++){
        Particle flowi(mRad, Color(35,137,218));
        flowi.m = mass;
        flowi.set_pos(iniPos,linGap*i);
        flowi.set_vel(iniVx,iniVy);

        flows.push_back(flowi);
    }

    return flows;
}

void partPotentials(vector<Particle>& parts, float dt){
    
    for (int i = 0; i < parts.size(); i++){
        for (int j = i+1; j < parts.size(); j++){
            float dx = parts[j].x - parts[i].x;
            float dy = parts[j].y - parts[i].y;
            float r  = sqrt(dx*dx + dy*dy); if (r<5.f) r=5.f;
            float theta = atan2(dy, dx);

            float a1 = -G*parts[j].m/pow(r,2.5);
            float a2 =  G*parts[i].m/pow(r,2.5);
            if (r>5){a1=0.f; a2=0.f;}
            else if (r<5){a1*=2;a2*=2;}
            

            parts[i].vx += a1*cos(theta)*dt;
            parts[i].vy += a1*sin(theta)*dt;
            parts[j].vx += a2*cos(theta)*dt;
            parts[j].vy += a2*sin(theta)*dt;
        }
    }
}

void objPotential(vector<Particle>& parts, vector<VertexArray>& objs, float dt){
    for (auto& p:parts){
        for (auto& obj:objs){
            for (int i=0;i<obj.getVertexCount();i++){
                float dx = obj[i].position.x - p.x;
                float dy = obj[i].position.y - p.y;
                float r  = sqrt(dx*dx+dy*dy); if (r<5.f) r=5.f;
                float theta = atan2(dy,dx);

                float a1 = -G*objMass/pow(r,2);
                if (r>10){a1*=-1*.001;}
                else if (r<5){a1*=2;}

                p.vx+=a1*cos(theta)*dt;
                p.vy+=a1*sin(theta)*dt;
            }
        }
    }
}

VertexArray line(vector<float> start, vector<float> end){
    float dx = end[0]-start[0];
    float dy = end[1]-start[1];

    float r  = sqrt(dx*dx+dy*dy);
    float theta = atan2(dy, dx);

    int pts = ceil(r/px);
    VertexArray points(PrimitiveType::Points,pts);
    for (int i=0;i<pts;i++){
        int resX = floor(dx/pts);
        int resY = floor(dy/pts);

        points[i].position = {start[0]+i*resX,start[1]+i*resY};
        points[i].color = Color::White;
    }

    return points;
}

VertexArray circ(vector<float> centre, float radius){
    float circumfrence = 2*pi*radius;
    int pts = ceil(circumfrence/px);
    float thetaDiv = 2*pi/pts;

    VertexArray points(PrimitiveType::Points,pts);
    for (int i=0;i<pts;i++){
        points[i].position = {centre[0] + radius*cos(i*thetaDiv), centre[1] + radius*sin(i*thetaDiv)};
    }

    return points;
}

vector<VertexArray> makeLada(){
    vector<VertexArray> objs;

    VertexArray l1 = line({620.f,420.f},{950.f,420.f}); objs.push_back(l1);
    VertexArray l2 = line({620.f,420.f},{600.f,330.f}); objs.push_back(l2);
    VertexArray l3 = line({600.f,330.f},{740.f,330.f}); objs.push_back(l3);
    VertexArray l4 = line({740.f,330.f},{800.f,250.f}); objs.push_back(l4);
    VertexArray l5 = line({800.f,250.f},{920.f,250.f}); objs.push_back(l5);
    VertexArray l6 = line({920.f,250.f},{950.f,330.f}); objs.push_back(l6);
    VertexArray l7 = line({950.f,330.f},{950.f,420.f}); objs.push_back(l7);

    VertexArray c1 = circ({690.0f,430.0f},40.0f);       objs.push_back(c1);
    VertexArray c2 = circ({890.0f,430.0f},40.0f);       objs.push_back(c2);

    return objs;
}

void setTrajectory(vector<Particle>& parts, float dt){
    for (auto& p : parts){
        p.x += p.vx*dt;
        p.y += p.vy*dt;
        p.ball.setPosition({p.x, p.y});
    }
}

void makeFlow(vector<Particle>& parts){
    vector<Particle> flows = flowParts();
    for (auto & flow:flows) {
        parts.push_back(flow);
    }
}

int main(){
    
    #pragma region CreateFlowTime

    // Track time
    Clock clock;
    Time lastTime = clock.getElapsedTime();
    Time lastFlow = lastTime;

    // Define Particles
    vector<Particle> movers; 

    // Create & open display window
    RenderWindow window(VideoMode({windowX,windowY}),"Flow");
    window.setFramerateLimit(60);

    #pragma endregion

    auto objs = makeLada();
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

        // Draw particles & background objects
        for (auto& mover:movers){mover.reDraw(); window.draw(mover.ball);}      
        for (auto& obj:objs)   {window.draw(obj);} 

        // Update positions
        partPotentials(movers,dt);
        objPotential(movers, objs,dt);
        setTrajectory(movers,dt);
        // cout << movers.size() <<endl; // Make sure particle sizes arent exploding

        // Define flow particles
        removeBounds(movers);
        if ((now-lastFlow).asSeconds() > tGap/1.2){makeFlow(movers); lastFlow = lastTime;}   
    }

    return 0;
}