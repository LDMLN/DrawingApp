#include "Spawner.h"

Spawner::Spawner() {
}

void Spawner::setup(int _x, int _y, int _size, ofColor _color, bool _moving = false) {
    x = _x;      
    y = _y;

    speedX = ofRandom(-5, 5);           
    speedY = ofRandom(-5, 5);

    size = _size;
    color = _color;
    moving = _moving;

}

void Spawner::update() {
    if (x < 0) {
        x = 0;
        speedX *= -1;
    }
    else if (x > 800 - size) {
        x = 800 - size;
        speedX *= -1;
    }

    if (y < 0) {
        y = 0;
        speedY *= -1;
    }
    else if (y > 600 - size) {
        y = 600 - size;
        speedY *= -1;
    }

    if (moving) {
        x += speedX;
        y += speedY;
    }
}

void Spawner::draw() {
    ofSetColor(color);
    ofDrawCircle(x, y, size);
}