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

		ofVec3f getPoint(float theta, float phi);
		ofVec3f cartesianToSpherical(ofVec3f point);
		float supershape(float theta, float m, float n1, float n2, float n3);
		ofVec3f scaleColorToUniform(ofColor col);
		ofVec3f calculateFaceNormal(ofVec3f A, ofVec3f B, ofVec3f C);
    
        vector<ofColor> getColorsFromImage(ofImage img);
    
        void drawColors(vector<ofColor> cols, int _x, int _y, float limit);
    
        void applyColorsToSupershape(vector<ofColor> cols);
    
        ofImage img;
        int imgIndex;
        vector<string> imagePaths;
    
        vector<ofColor> cols;

		ofShader light;
		ofShader rimLight;
		ofShader blurShader;

		ofEasyCam cam;
 
		ofMesh mesh;

		ofxPanel lightGui;

		ofParameterGroup lighting;

		ofFbo rimBuffer;
		ofFbo objectBuffer;
		ofFbo blurBuffer;

		ofParameter<ofVec3f> lightPos;
		ofParameter<ofColor> m_ambient;
		ofParameter<ofColor> m_diffuse;
		ofParameter<ofColor> m_specular;
		ofParameter<ofColor> m_emissive;
		ofParameter<float> shininess;
		ofParameter<ofColor> l_ambient;
		ofParameter<ofColor> l_diffuse;
		ofParameter<ofColor> l_specular;
		ofParameter<float> blur;
		ofParameter<bool> showGui;

		ofxPanel supershapeGui;
		ofParameter<bool> drawNormals;

		ofParameter<float> scale;
        ofParameter<float> speed;
        ofParameter<float> noiseScale;
        ofParameter<float> noiseResolution;

		ofParameterGroup supershapeGroup1;

		ofParameter<int> m1;
		ofParameter<float> n11;
		ofParameter<float> n21;
		ofParameter<float> n31;
		ofParameter<float> a1;
		ofParameter<float> b1;

		ofParameterGroup supershapeGroup2;

		ofParameter<float> m2;
		ofParameter<float> n12;
		ofParameter<float> n22;
		ofParameter<float> n32;
		ofParameter<float> a2;
		ofParameter<float> b2;

};
