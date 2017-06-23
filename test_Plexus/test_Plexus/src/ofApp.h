#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxNestedFileLoader.h"

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

		ofShader metaballs;
		ofShader maskShader;

		ofFbo buffer;

		vector<ofImage*> masks;

		ofVideoGrabber cam;
		int maskIndex;
		
		ofxPanel gui;

		bool exploding;

		vector<ofVec4f> ballPositions;

		ofParameter<float> radius;
		ofParameter<float> noise;
		ofParameter<float> camDist;

};
