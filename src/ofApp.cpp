#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <vector>

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

	//main menu option explanations
	explanationParameters.setName("explanation");
	explanationParameters.add(drawingAppExplanation.set("This allows you to draw manually", "This allows you to draw manually"));
	explanationParameters.add(randomDrawingExplanation.set("Will randomly generate a drawing,\nmay take several seconds", "Will randomly generate a drawing,\nmay take several seconds"));

	menuExplanations.setDefaultHeight(25);
	menuExplanations.setup(explanationParameters);
	menuExplanations.setPosition(mainMenu.getPosition().x - 300, mainMenu.getPosition().y +25);

	//exit menu
	exitOptions.setName("Exit Options");
	exitOptions.add(toMain.set("Main Menu", false));
	exitOptions.add(exitApp.set("Exit app", false));


	exitMenu.setup(exitOptions);
	exitMenu.setPosition(WIDTH - exitMenu.getWidth(), HEIGHT - exitMenu.getHeight());


	//brush setting parameters
	sliderGroup.setName("brush and canvas settings");
	sliderGroup.add(brushSize.set("brush size", 20, 3, 64));
	sliderGroup.add(unfilled.set("outline brush", false));
	sliderGroup.add(brushType.set("brush type", 1, 1, 3));
	sliderGroup.add(rotateTriangles.set("rotate triangles", false));
	sliderGroup.add(colorSlider.set("color", ofColor(255, 255, 255, 255), ofColor(0,0,0,0), ofColor(255, 255,255,255)));
	sliderGroup.add(backgroundColor.set("background", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255)));

	//effect parameters
	effectsGroup.setName("effect settings");
	effectsGroup.add(randomizeColors.set("randomize colors", false));
	effectsGroup.add(btnClear.set("clear canvas", false));
	effectsGroup.add(spawnFromBrush.set("spawn from brush", false));
	effectsGroup.add(randomize.set("randomize", false));

	mainGroup.add(sliderGroup);
	mainGroup.add(effectsGroup);

	gui.setDefaultHeight(15);
	gui.setup(mainGroup);
	gui.setPosition(WIDTH - gui.getWidth(), 10);

	fbo.allocate(canvasWidth, canvasHeight);
	fbo.begin();
	ofClear(255);
	fbo.end();

	//warning gui
	warningsHolder.setName("Warnings and Advice");
	warningsHolder.add(spawnerWarning.set("Using the spawner option\nmay result in a\nslight slowdown of the program.\n\nPlease keep that in mind."));
	warningsHolder.add(eraserAdvice.set("You may erase by\nholding the right mouse button\nor by simply pressing the\n'del' key if you want to start fresh"));

	warningsPanel.disableHeader();
	warningsPanel.setDefaultHeight(120);
	warningsPanel.setDefaultWidth(300);
	helpWanted.setDefaultBorderColor(ofColor::red);
	helpWanted.setDefaultBackgroundColor(ofColor::pink);
	helpWanted.setDefaultFillColor(ofColor::grey);
	helpWanted.setHeaderBackgroundColor(ofColor::pink);
	helpWanted.setDefaultTextColor(ofColor::black);
	warningsPanel.setup(warningsHolder);
	warningsPanel.setPosition(WIDTH - warningsPanel.getWidth(), HEIGHT - 700);

	//help wanted menu
	helpButtonHolder.setName("HELP MENU");
	helpButtonHolder.add(helpNeeded.set("Mode Explanation", false));
	helpButtonHolder.add(warnings.set("warnings", false));

	helpWanted.setDefaultHeight(25);
	helpWanted.setDefaultWidth(220);
	helpWanted.setDefaultBorderColor(ofColor::red);
	helpWanted.setDefaultBackgroundColor(ofColor::pink);
	helpWanted.setDefaultFillColor(ofColor::grey);
	helpWanted.setHeaderBackgroundColor(ofColor::pink);
	helpWanted.setDefaultTextColor(ofColor::black);
	helpWanted.setup(helpButtonHolder);
	helpWanted.setPosition(WIDTH - helpWanted.getWidth(), HEIGHT - helpWanted.getHeight());

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
		btnClear = true;
		toMain = false;
		drawingApp = false;
		randomDrawing = false;
		randomBrushCallLimiter = false;
		backgroundColor = glm::vec3(0, 0, 0);
	}

	if (randomize) {
		randomize = false;
		//makeRequest();
		randomBrush();
		//generateImage();
	}

	if (exitApp) {
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (!drawingApp && !randomDrawing) {
		fbo.draw(0, 0);
		mainMenu.draw();
		helpWanted.draw();
		if (helpNeeded) {
			menuExplanations.disableHeader();
			menuExplanations.draw();
		}
		if (warnings) {
			warningsPanel.draw();
		}
	}

	if (drawingApp) {
		fbo.draw(0, 0);
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(0, 0, canvasWidth, canvasHeight);
		ofFill();
		gui.draw();
		exitMenu.draw();
		if (groupOfSpawners.size() > 0) {
			if (unfilled) {
				ofNoFill();
			}
			else {
				ofFill();
			}
			for (int i = 0; i < groupOfSpawners.size(); i++) {
				groupOfSpawners[i].draw();
			}
		}
	}
	if (randomDrawing) {
		fbo.draw(0, 0);
		ofNoFill();
		ofSetColor(255);
		ofDrawRectangle(0, 0, canvasWidth, canvasHeight);
		ofFill();
		gui.draw();
		exitMenu.draw();

		if (!randomBrushCallLimiter) {
			generateImage();
			generateImage();
			randomBrushCallLimiter = true;
		}

		if (groupOfSpawners.size() > 0) {
			if (unfilled) {
				ofNoFill();
			}
			else {
				ofFill();
			}
			for (int i = 0; i < groupOfSpawners.size(); i++) {
				groupOfSpawners[i].draw();
			}
		}
	}
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::drawWithBrush(int x, int y) {
	//sets color of brush depending on if randomizeColors toggled or not
	ofColor brushColor = randomizeColors ? ofColor(ofRandom(255), ofRandom(255), ofRandom(255)) : colorSlider;
		//rectangle brush
		if (brushType == 1) {
			Spawner tempRect;
			if (x <= fbo.getWidth() && y <= fbo.getHeight()) {
				tempRect.setup(x, y, brushSize, brushColor, brushType, spawnFromBrush, ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0));
				groupOfSpawners.push_back(tempRect);
			}
		}
		//circle brush
		else if (brushType == 2) {
			Spawner tempCircle;
			if (x <= fbo.getWidth() && y <= fbo.getHeight()) {
				tempCircle.setup(x, y, brushSize, brushColor, brushType, spawnFromBrush, ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0));
				groupOfSpawners.push_back(tempCircle);
			}
		}
		//triangle brush
		else if (brushType == 3) {
			ofVec2f mousePos(x, y);

			//spawns triangle at origin to make easier for rotating
			ofVec2f tPoint1(0, brushSize);
			ofVec2f tPoint2(brushSize * 4, 0);
			ofVec2f tPoint3(0, -brushSize);

			//rotates based on current compass rose direction of mouse
			if (rotateTriangles) {
				float rotation = findDirection(x, y);

				tPoint1.rotate(rotation);
				tPoint2.rotate(rotation);
				tPoint3.rotate(rotation);
			}
			//moves triangle to mouse position
			tPoint1 += mousePos;
			tPoint2 += mousePos;
			tPoint3 += mousePos;

			Spawner tempTri;
			if (x <= fbo.getWidth() && y <= fbo.getHeight()) {
				tempTri.setup(x, y, brushSize, brushColor, brushType, spawnFromBrush, tPoint1, tPoint2, tPoint3);
				groupOfSpawners.push_back(tempTri);
			}
		}
}
//---------------------------------------------------------------

//returns the current direction the mouse is moving
//used when drawing triangle to have them face the correct angle.
float ofApp::findDirection(int x, int y) {
	float angle_to_return = atan2(-ofGetPreviousMouseY() - (-y), -ofGetPreviousMouseX() - (-x));

	return angle_to_return * 180 / PI;
}

void ofApp::eraseWithEraser(int x, int y) {
	//erase point that mouse hovers over
	for (int i = 0; i < groupOfSpawners.size(); i++) {
		float distance = ofDist(x, y, groupOfSpawners[i].x, groupOfSpawners[i].y);
		if (distance < groupOfSpawners[i].size) {
			groupOfSpawners.erase(groupOfSpawners.begin() + i);
		}
	}

}

//---------------------------------------------------------------
void ofApp::keyPressed(int key){
	//take screenshot
	if (key == ' ') {
		imgScreenshot.grabScreen(0, 0, fbo.getWidth(), fbo.getHeight());
		string filename = "screenshot_" + ofGetTimestampString() + ".png";
		imgScreenshot.save(filename);
	}

	//clear board
	if (key == OF_KEY_DEL) {
		btnClear = true;
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
	if (drawingApp || randomDrawing) {
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
	if (drawingApp || randomDrawing) {
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
//--------------------------------------------------------------
//used in a prior version of this app that involved file writing/reading and calling a microservice.
/*
void ofApp::makeRequest() {
	ofstream myFile("G:\\OSUcourses\\CS361\\randomPickMicroservice\\options.txt");

	myFile << "REQUEST";

	myFile.close();

	ofstream myCoordFile("G:\\OSUcourses\\CS361\\randomPickMicroservice\\coordinates.txt");

	myCoordFile << "REQUEST";

	myCoordFile.close();

}
*/

//--------------------------------------------------------------
void ofApp::randomBrush() {
	brushType = ofRandom(1, 4);
	brushSize = ofRandom(3, 65);
	colorSlider = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
	backgroundColor = glm::vec3(ofRandom(255), ofRandom(255), ofRandom(255));
	//low chance of randomly having the spawn feature be toggled
	spawnFromBrush = (int)ofRandom(20) == 1 ? 1 : 0;
	if (brushType == 3) {
		rotateTriangles = (int)ofRandom(2);
	}

	//the following is an implementation of a version of this app that I had made previously that 
	//incorporated a microservice and file reading.
	
	/*
	for (int i = 0; i < 10000000; i++) {
		continue;
	}

	std::string myText;

	//checks where the file is currently working from 
	//std::cout << std::filesystem::current_path().string() << std::endl;

	std::ifstream myReadFile("G:\\OSUcourses\\CS361\\randomPickMicroservice\\options.txt");

	getline(myReadFile, myText);

	//std::cout << myText;

	convertStringToIntArray(myText);

	for (int i = 0; i < 10; i++) {
		//std::cout << brushArray[i] << "\n";
	}
	brushType = brushArray[0];
	brushSize = brushArray[1];
	colorSlider = ofColor(brushArray[2], brushArray[3], brushArray[4], brushArray[5]);
	backgroundColor = glm::vec3(brushArray[6], brushArray[7], brushArray[8]);

	//std::cout << myText;


	myReadFile.close();
	*/
}
//--------------------------------------------------------------
//used in a prior version of this app that involved file writing/reading and calling a microservice.
/*
void ofApp::convertStringToIntArray(string str) {
	int str_length = str.length();
	std:string num_builder = "";
	int j = 0;

	for (int i = 0; i < str_length; i++) {
		if (str[i] == ',') {
			brushArray[j] = stoi(num_builder);
			//brushArray[j] = str[i] - '0';
			j++;
			num_builder = "";
		}
		else {
			num_builder += str[i];
			if (i + 1 == str_length) {
				brushArray[j] = str[i] - '0';
			}
		}
	}
}
*/
//--------------------------------------------------------------
void ofApp::generateImage() {
	//makeRequest();
	randomBrush();
	spawnFromBrush = 0;

	fbo.begin();
	for (int i = 0; i < (int)ofRandom(20, 120); i++) {
		colorSlider = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
		ofPoint p0((int)ofRandom(1000), (int)ofRandom(761));
		ofPoint p1((int)ofRandom(1000), (int)ofRandom(761));


			//Bresenham's line algorithm
			//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
			//modifed to work in c++ and openframeworks
			int dx = abs(p1.x - p0.x);
			int sx = p0.x < p1.x ? 1 : -1;
			int dy = -abs(p1.y - p0.y);
			int sy = p0.y < p1.y ? 1 : -1;
			int error = dx + dy;
			//these two next values help to add interesting spacing to the image, 
			// not part of Bresenham's equation
			int skip_count = 0;
			int random_skipper = ofRandom(1, 75);

			while (true) {
				if (skip_count % random_skipper == 0) {
					drawWithBrush(p0.x, p0.y);
				}
				skip_count += 1;
				if (p0.x == p1.x && p0.y == p1.y) {
					break;
				}
				int e2 = 2 * error;
				if (e2 >= dy) {
					if (p0.x == p1.x) {
						break;
					}
					error = error + dy;
					p0.x = p0.x + sx;
				}
				if (e2 <= dx) {
					if (p0.y == p1.y) {
						break;
					}
					error = error + dx;
					p0.y = p0.y + sy;
				}
			}
		}
	fbo.end();

	//old code from a version of this app that used a microservice and file reading/writing

	/*
	for (int i = 0; i < 1000000000; i++) {
		continue;
	}

	std::string myText;

	//checks where the file is currently working from
	//std::cout << std::filesystem::current_path().string() << std::endl;

	std::ifstream myReadFile("G:\\OSUcourses\\CS361\\randomPickMicroservice\\coordinates.txt");

	int m = 0;

	coordinateArray.clear();

	while (getline(myReadFile, myText, ',')) {
		coordinateArray.push_back(stoi(myText));
		//std::cout << "myText: " << myText << "\n";
		//std::cout << "coordinate Array: " << coordinateArray[m] << "\n";
		m++;
	}

	/*
	for (int j = 0; j < coordinateArray.size(); j++) {
		std::cout << coordinateArray[j] << "\n";
	}


	//std::cout << coordinateArray[coordinateArray.size()-1] << "\n";
	fbo.begin();
	/*
	for (int i = 0; i < coordinateArray.size(); i += 2) {
		colorSlider = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
		if (i + 3 < coordinateArray.size()) {
			ofPoint p0(coordinateArray[i], coordinateArray[i + 1]);
			ofPoint p1(coordinateArray[i + 2], coordinateArray[i + 3]);
	*/
}

//--------------------------------------------------------------
