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
		float findDirection(int x, int y);
		
		//random drawing functions
		void randomBrush();
		void convertStringToIntArray(string str);
		void makeRequest();

		void generateImage();

		int brushArray[12];
		vector<int> coordinateArray;
		//int coordinateArray[100];
		int guiWidth = 200;

		bool randomBrushCallLimiter = false;


		//--------------------------------
		//			Main menu gui
		ofxPanel mainMenu;
		ofParameterGroup menuSelect;

		ofParameter<bool> drawingApp;
		ofParameter<bool> randomDrawing;
		//---------------------------------
		//			Main menu helper gui
		ofxPanel menuExplanations;
		ofParameterGroup explanationParameters;

		ofParameter<std::string> drawingAppExplanation;
		ofParameter<std::string> randomDrawingExplanation;
		//---------------------------------
		//			Warnings gui
		ofxPanel warningsPanel;
		ofParameterGroup warningsHolder;

		ofParameter<std::string> spawnerWarning;
		ofParameter<std::string> eraserAdvice;

		//---------------------------------
		//			help wanted gui
		ofxPanel helpWanted;
		ofParameterGroup helpButtonHolder;

		ofParameter<bool> helpNeeded;
		ofParameter<bool> warnings;
		//---------------------------------
		// 
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
		ofParameter<bool> rotateTriangles;
		ofParameter<bool> unfilled;
		ofParameter<ofColor> colorSlider;
		ofParameter<glm::vec3> backgroundColor;
		ofParameter<bool> btnClear;
		ofParameter<bool> randomize;

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
		vector<Spawner> eraserGroup;
		//----------------------------------------
};
