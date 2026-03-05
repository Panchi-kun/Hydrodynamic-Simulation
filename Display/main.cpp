#include <SFML/Graphics.hpp>
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>

unsigned int windowX = 400;
unsigned int windowY = 400;
int particleNum = 2;

float g = 90.8f;
float rad = 20.f;

float* ballLocational(float dt, float* info){
    info[3] += g*dt;
    info[0] += info[2]*dt;
    info[1] += info[3]*dt;
    
    return info;
}

float* wallCollision(float* info){
    if (info[0] <= 0 || info[0]+2*rad >= windowX) info[2] = -info[2];
    if (info[1] <= 0 || info[1]+2*rad >= windowY) info[3] = -info[3];
    return info;
}

float* ballCollision(float symInfo){

}

float* makeInitial(float* infoi){
    static std::default_random_engine gen(std::random_device{}());
    std::uniform_real_distribution<double> rPos(0.0,std::min(windowX-2*rad,windowY-2*rad));
    std::uniform_real_distribution<double> rVel(0.0,100.0);

    infoi[0] = rPos(gen); infoi[2] = rVel(gen);
    infoi[1] = rPos(gen); infoi[3] = rVel(gen);
    return infoi;
}

sf::CircleShape createBall(){
    sf::CircleShape ball(rad); ball.setFillColor(sf::Color(150,50,50));
    return ball;
}

int main() {

    sf::RenderWindow window(sf::VideoMode({windowX,windowY}),"My Windows");
    window.setFramerateLimit(60);

    float info[particleNum][4];
    
    for (int i = 0; i < particleNum; i++){
        float infoi[4];
        makeInitial(infoi);
        info[i][0] = infoi[0]; info[i][2] = infoi[2];
        info[i][1] = infoi[1]; info[i][3] = infoi[3];
    }

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
        
        

        
        

        //ballLocational(dt,info[0]);
        //wallCollision(info1);
            
        //ballLocational(dt,info2);
        //wallCollision(info2);

        sf::CircleShape ball1 = createBall();
        sf::CircleShape ball2 = createBall();

        for (int i=0; i<particleNum; i++){
            float* infoi = info[i];
            ballLocational(dt, infoi);
            wallCollision(infoi);
        }

        int dealt[particleNum][particleNum] = {};
        for (int i=0; i<particleNum; i++){
            for (int j=0; j<particleNum; j++){
                if (i==j || dealt[i][j] == 1) continue;
                dealt[i][j]=1;
                dealt[j][i]=1;

                float dx = abs(info[i][0] - info[j][0]);
                float dy = abs(info[i][1] - info[j][1]);
                
                float Ux = dx - 2*rad;
                float Uy = dy - 2*rad;
                
                if (Ux<0 && Uy<0){
                    float nUx = 0.5f*abs(Ux);
                    float nUy = 0.5f*abs(Uy);

                    info[i][2] = info[i][2] - nUx; info[j][2] = info[j][2] + nUx;
                    info[i][3] = info[i][3] - nUy; info[j][3] = info[j][3] + nUy;
                    
                    
                }

            }
        }

        ball1.setPosition({info[0][0],info[0][1]});
        ball2.setPosition({info[1][0],info[1][1]});

        window.draw(ball1);   
        window.draw(ball2);

        window.display();
    
    }

    return 0;
}