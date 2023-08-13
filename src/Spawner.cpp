#include "Spawner.h"

Spawner::Spawner() {
}
void Spawner::setup(int _x, int _y, int _size, ofColor _color, int _brushType, bool _moving = false, ofVec2f _tPoint1 = ofVec2f(0, 0), ofVec2f _tPoint2 = ofVec2f(0, 0), ofVec2f _tPoint3 = ofVec2f(0,0)) {
    tPoint1 = _tPoint1;
    tPoint2 = _tPoint2;
    tPoint3 = _tPoint3;

    triangleSpeed = ofVec2f(ofRandom(-5, 5), ofRandom(-5,5));

    x = _x;      
    y = _y;

    speedX = ofRandom(-5, 5);           
    speedY = ofRandom(-5, 5);

    brushType = _brushType;
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
    if (brushType == 3) {
        if (tPoint1.x < 0 || tPoint2.x < 0 || tPoint3.x < 0) {
            if (tPoint1.x < 0) {
                tPoint1.x = 0;
            }
            if (tPoint2.x < 0) {
                tPoint2.x = 0;
            }
            if (tPoint3.x < 0) {
                tPoint3.x = 0;
            }
            triangleSpeed *= ofVec2f(-1, 1);
        }
        else if (tPoint1.x > 800 - size || tPoint2.x > 800 - size || tPoint3.x > 800 - size) {
            if (tPoint1.x > 800 - size) {
                tPoint1.x = 800 - size;
            }
            if (tPoint2.x > 800 - size) {
                tPoint2.x = 800 - size;
            }
            if (tPoint3.x > 800 - size) {
                tPoint3.x = 800 - size;
            }
            triangleSpeed *= ofVec2f(-1, 1);
        }

        else if (tPoint1.y < 0 || tPoint2.y < 0 || tPoint3.y < 0) {
            if (tPoint1.y < 0) {
                tPoint1.y = 0;
            }
            if (tPoint2.y < 0) {
                tPoint2.y = 0;
            }
            if (tPoint3.y < 0) {
                tPoint3.y = 0;
            }
            triangleSpeed *= ofVec2f(1, -1);
        }

        else if (tPoint1.y > 600 - size || tPoint2.y > 600 - size || tPoint3.y > 600 - size) {
            if (tPoint1.y > 600 - size) {
                tPoint1.y = 600 - size;
            }
            if (tPoint2.y > 600 - size) {
                tPoint2.y = 600 - size;
            }
            if (tPoint3.y > 600 - size) {
                tPoint3.y = 600 - size;
            }
            triangleSpeed *= ofVec2f(1, -1);
        }
    }
    if (moving) {
        x += speedX;
        y += speedY;

        tPoint1 += triangleSpeed;
        tPoint2 += triangleSpeed;
        tPoint3 += triangleSpeed;
    }

    
}

void Spawner::draw() {
    ofSetColor(color);
    if (brushType == 1) {
        ofDrawRectangle(x, y, size, size);
    }
    else if (brushType == 2) {
        ofDrawCircle(x, y, size);
    }

    else if (brushType == 3) {
        ofDrawTriangle(tPoint1, tPoint2, tPoint3);
    }
}