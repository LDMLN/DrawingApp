#pragma once


#include "ofMain.h"

class Spawner {

public: 
   
    void setup(int _x, int _y, int _size,  ofColor _color, bool _moving);
    void update();  
    void draw();    


    float x;        
    float y;
    float speedY;   
    float speedX;
    int size;      
    int _size;
    int boundaryW;
    int boundaryH;
    int _x;
    int _y;
    ofColor color;
    ofColor _color;

    bool moving;
    bool _moving;

    Spawner(); 
private: 
}; 