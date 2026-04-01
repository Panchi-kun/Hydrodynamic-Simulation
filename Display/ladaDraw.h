#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
using namespace sf;

const int px  = 10;

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

vector<VertexArray> objs;

VertexArray l1 = line({620.f,420.f},{950.f,420.f});
VertexArray l2 = line({620.f,420.f},{600.f,330.f});
VertexArray l3 = line({600.f,330.f},{740.f,330.f});
VertexArray l4 = line({740.f,330.f},{800.f,250.f});
VertexArray l5 = line({800.f,250.f},{920.f,250.f});
VertexArray l6 = line({920.f,250.f},{950.f,330.f});
VertexArray l7 = line({950.f,330.f},{950.f,420.f});
VertexArray c1 = circ({690.0f,430.0f},40.0f);
VertexArray c2 = circ({890.0f,430.0f},40.0f);

objs.push_back(l1); objs.push_back(l2); objs.push_back(l3); 
objs.push_back(l4); objs.push_back(l5); objs.push_back(l6);
objs.push_back(l7);

objs.push_back(c1); objs.push_back(c2);