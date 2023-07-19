#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableSmoothing();
	//main menu
	menuSelect.setName("Main Menu");
	menuSelect.add(drawingApp.set("drawing app", false));
	menuSelect.add(randomDrawing.set("random drawing", false));

	mainMenu.setDefaultHeight(25);
	mainMenu.setup(menuSelect);
	mainMenu.setPosition(WIDTH / 2 - mainMenu.getWidth()/2, HEIGHT / 2 - mainMenu.getHeight()/2);

	//exit menu
	exitOptions.setName("Exit Options");
	exitOptions.add(toMain.set("Main Menu", false));
	exitOptions.add(exitApp.set("Exit app", false));


	exitMenu.setup(exitOptions);
	exitMenu.setPosition(WIDTH - exitMenu.getWidth(), HEIGHT - exitMenu.getHeight());


	//brush setting parameters
	sliderGroup.setName("brush and canvas settings");
	sliderGroup.add(brushSize.set("brush size", 20, 3, 64));
	sliderGroup.add(floatSlider.set("float slider", 32, 32, 256));
	sliderGroup.add(eraseAfterDraw.set("erase after drawing", false));
	sliderGroup.add(brushType.set("brush type", 1, 1, 4));
	sliderGroup.add(colorSlider.set("color", ofColor(255, 255, 255, 255), ofColor(0,0,0,0), ofColor(255, 255,255,255)));
	sliderGroup.add(backgroundColor.set("background", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255)));

	//effect parameters
	effectsGroup.setName("effect settings");
	effectsGroup.add(randomizeColors.set("randomize colors", false));
	effectsGroup.add(btnClear.set("clear canvas", false));
	effectsGroup.add(spawnFromBrush.set("spawn from brush", false));

	mainGroup.add(sliderGroup);
	mainGroup.add(effectsGroup);

	gui.setDefaultHeight(15);
	gui.setup(mainGroup);
	gui.setPosition(WIDTH - gui.getWidth(), 10);

	fbo.allocate(canvasWidth, canvasHeight);
	fbo.begin();
	ofClear(255);
	fbo.end();

}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(backgroundColor->x, backgroundColor->y, backgroundColor->z);
	if (btnClear) {
		fbo.begin();
		ofClear(255);
		fbo.end();
		btnClear = false;
		groupOfSpawners.clear();
	}
	for (int i = 0; i < groupOfSpawners.size(); i++) {
		groupOfSpawners[i].update();
	}

	if (toMain) {
		toMain = false;
		drawingApp = false;
	}

	if (exitApp) {
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (!drawingApp) {
		mainMenu.draw();
		btnClear = true;
	}
	if (drawingApp) {
		fbo.draw(0, 0);
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(0, 0, canvasWidth, canvasHeight);
		ofFill();
		gui.draw();
		exitMenu.draw();
		for (int i = 0; i < groupOfSpawners.size(); i++) {
			groupOfSpawners[i].draw();
		}
	}
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::drawWithBrush(int x, int y) {

	//if you want to make the brushes 1-3 not be overridden when brush 4 draws, make them classes like 4. 
	// (should only have 1-3 brushes then, one for each shape, as subclasses of Spawner).

	//draws with color selected
	if (!randomizeColors) {
		ofSetColor(colorSlider);
		if (brushType == 1) {
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofDrawRectangle(x, y, brushSize, brushSize);
		}
		else if (brushType == 2) {
			ofDrawCircle(x, y, brushSize);
		}
		else if (brushType == 3) {
			ofDrawTriangle(glm::vec2(x, y + brushSize), glm::vec2(x + brushSize * 4, y),
				glm::vec2(x, y - brushSize));
		}
		else if (brushType == 4) {
			Spawner tempSpawn;
			tempSpawn.setup(x, y, brushSize, colorSlider, spawnFromBrush);
			groupOfSpawners.push_back(tempSpawn);
			//tempSpawn.draw(); this was here when it worked before adding the "moving" argument to setup
		}
	}
	//randomized colors
	else if (randomizeColors) {
		ofColor randomColor = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
		ofSetColor(randomColor);
		if (brushType == 1) {
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofDrawRectangle(x, y, brushSize, brushSize);
		}
		else if (brushType == 2) {
			ofDrawCircle(x, y, brushSize);
		}
		else if (brushType == 3) {
			ofDrawTriangle(glm::vec2(x, y + brushSize), glm::vec2(x + brushSize * 4, y),
				glm::vec2(x, y - brushSize));
		}
		else if (brushType == 4) {
			Spawner tempSpawn;
			if (x <= fbo.getWidth() - brushSize && y <= fbo.getHeight() - brushSize) {
				tempSpawn.setup(x, y, brushSize, randomColor, spawnFromBrush);
				groupOfSpawners.push_back(tempSpawn);
			}
			//tempSpawn.draw();
		}


	}
}
//---------------------------------------------------------------

void ofApp::eraseWithEraser(int x, int y) {
	//this is just drawing the background color over it so when you change background color it's visible
	//thinking of a way to fix this would be a nice addition.
	ofDrawCircle(x, y, brushSize);

}

//---------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		imgScreenshot.grabScreen(0, 0, fbo.getWidth(), fbo.getHeight());
		string filename = "screenshot_" + ofGetTimestampString() + ".png";
		imgScreenshot.save(filename);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (drawingApp) {
		fbo.begin();
		if (button == OF_MOUSE_BUTTON_LEFT) {
			drawWithBrush(x, y);
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT) {
			ofSetColor(backgroundColor->x, backgroundColor->y, backgroundColor->z);
			eraseWithEraser(x, y);
		}
		fbo.end();
	}

	

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (drawingApp) {
		fbo.begin();
		if (button == OF_MOUSE_BUTTON_LEFT) {
			drawWithBrush(x, y);
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT) {
			ofSetColor(backgroundColor->x, backgroundColor->y, backgroundColor->z);
			eraseWithEraser(x, y);
		}
		fbo.end();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
