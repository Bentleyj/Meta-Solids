#include "ofApp.h"

#define NUM_BALLS 100

//--------------------------------------------------------------
void ofApp::setup(){

	metaballs.load("shaders/metaSolids");

	gui.setup("settings", "settings/settings.xml");
	gui.add(radius.set("Radius", 1.0, 0.0, 4.0));
	gui.add(noise.set("Noise", 1.0, 0.0, 4.0));
	gui.add(camDist.set("Camera Distance", 4.0, 0.0, 10.0));
	gui.loadFromFile("settings/settings.xml");

	for (int i = 0; i < NUM_BALLS; i++) {
		float x = ofRandom(-2.0, 2.0);
		float y = ofRandom(-2.0, 2.0);
		float z = ofRandom(-2.0, 2.0);
		float r = 5.0;
		ballPositions.push_back(ofVec4f(x, y, z, r));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < ballPositions.size(); i++) {
		ballPositions[i].y -= 0.02;
		ballPositions[i].w -= 0.02;
		if (ballPositions[i].y < -5.0) {
			ballPositions[i].y = 0.0;
			ballPositions[i].w = 5.0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	metaballs.begin();
	metaballs.setUniform2f("u_Resolution", ofGetWidth(), ofGetHeight());
	metaballs.setUniform2f("u_Mouse", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
	metaballs.setUniform1f("u_Time", ofGetElapsedTimef());
	metaballs.setUniform1f("u_Radius", radius);
	metaballs.setUniform1f("u_Noise", noise);
	metaballs.setUniform1f("u_CameraDistance", camDist);
	metaballs.setUniform4fv("u_BallPositions", (float*)&ballPositions[0], ballPositions.size());
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	metaballs.end();

	gui.draw();

	ofSetColor(0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20);

	//ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
