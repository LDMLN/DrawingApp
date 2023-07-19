#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Spawner.h"
#define WIDTH 1024
#define HEIGHT 768

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawWithBrush(int x, int y);
		void eraseWithEraser(int x, int y);

		int guiWidth = 200;

		//--------------------------------
		//			Main menu gui
		ofxPanel mainMenu;
		ofParameterGroup menuSelect;

		ofParameter<bool> drawingApp;
		ofParameter<bool> randomDrawing;
		//---------------------------------
		//			Exit menu
		ofxPanel exitMenu;

		ofParameterGroup exitOptions;

		ofParameter<bool> exitApp;
		ofParameter<bool> toMain;


		//--------------------------------
		//			Drawing app gui
		ofxPanel gui;

		ofParameterGroup sliderGroup;
		ofParameter<int> brushSize;
		ofParameter<int> brushType;
		ofParameter<float> floatSlider;
		ofParameter<bool> eraseAfterDraw;
		ofParameter<ofColor> colorSlider;
		ofParameter<glm::vec3> backgroundColor;
		ofParameter<bool> btnClear;

		ofParameterGroup effectsGroup;
		ofParameter<bool> randomizeColors;
		ofParameter<bool> spawnFromBrush;

		ofParameterGroup mainGroup;

		ofImage imgScreenshot;
		int count;

		ofFbo fbo;
		int canvasWidth = 800;
		int canvasHeight = 600;

		bool spawnerExist;

		Spawner mySpawner;

		vector<Spawner> groupOfSpawners;
		//----------------------------------------
};
