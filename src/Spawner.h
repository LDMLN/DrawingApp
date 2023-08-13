#pragma once


#include "ofMain.h"

class Spawner {

public: 
   
    void setup(int _x, int _y, int _size,  ofColor _color, int _brushType, bool _moving, ofVec2f _tPoint1, ofVec2f _tPoint2, ofVec2f _tPoint3);
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
    int _type;
    int type;
    ofColor color;
    ofColor _color;
    int brushType;
    int _brushType;

    ofVec2f _tPoint1;
    ofVec2f tPoint1;
    ofVec2f _tPoint2;
    ofVec2f tPoint2;
    ofVec2f _tPoint3;
    ofVec2f tPoint3;

    ofVec2f triangleSpeed;

    bool moving;
    bool _moving;

    Spawner(); 
private: 
}; 