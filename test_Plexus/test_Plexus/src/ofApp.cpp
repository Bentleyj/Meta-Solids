#include "ofApp.h"

#define NUM_BALLS 100

//--------------------------------------------------------------
void ofApp::setup(){

	ofHideCursor();

	ofxNestedFileLoader loader;
	vector<string> imageNames = loader.load("images");

	for (int i = 0; i < imageNames.size(); i++) {
		ofImage* img = new ofImage();
		bool loaded = img->load(imageNames[i]);
		img->resize(ofGetWidth(), ofGetHeight());
		if(loaded)
			masks.push_back(img);
	}

	maskIndex = 0;

	buffer.allocate(ofGetWidth(), ofGetHeight());

	maskShader.load("shaders/mask");

	metaballs.load("shaders/metaSolids");
	exploding = false;

	gui.setup("settings", "settings/settings.xml");
	gui.add(radius.set("Radius", 1.0, 0.0, 4.0));
	gui.add(noise.set("Noise", 1.0, 0.0, 20.0));
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

	if (exploding) {
		noise += 0.1;
		if (noise > 10.0) noise = 10.0;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//buffer.begin();
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
	//buffer.end();

	//maskShader.begin();
	//maskShader.setUniform2f("u_Resolution", ofGetWidth(), ofGetHeight());
	//maskShader.setUniformTexture("u_TextureMask", *(masks[maskIndex]), 0);
	//maskShader.setUniformTexture("u_TextureBackground", buffer.getTexture(), 1);
	//ofPushMatrix();
	//ofSetColor(255);
	//masks[maskIndex]->draw(0, 0);
	//ofPopMatrix();
	//maskShader.end();

	//gui.draw();

	//ofSetColor(0);
	//ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20);

	//ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		exploding = true;
	}
	else if (key == 'p') {
		maskIndex++;
		maskIndex %= masks.size();
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
