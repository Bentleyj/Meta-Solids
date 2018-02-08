#include "ofApp.h"

#define NUM_STEPS 1000

//--------------------------------------------------------------
float getColorDistance(ofColor col1, ofColor col2) {
    float r = col2.r - col1.r;
    float g = col2.g - col1.g;
    float b = col2.b - col1.b;
    
    float dist = sqrt(r*r + g*g + b*b);
    
    return dist;
}

//--------------------------------------------------------------
ofColor getBlendedColor(ofColor col1, int w1, ofColor col2, int w2) {
    float r = col2.r * w2 + col1.r * w1;
    float g = col2.g * w2 + col1.g * w1;
    float b = col2.b * w2 + col1.b * w1;
    
    r /= (w1 + w2);
    g /= (w1 + w2);
    b /= (w1 + w2);

    return ofColor(r, g, b);
}

//--------------------------------------------------------------
void ofApp::setup(){

	cam.setPosition(0, 0, 5);

	mesh = mesh.sphere(1, NUM_STEPS);

	showGui = true;

	string lightSettingsFile = "settings/lighting.xml";
	lightGui.setup("Lighting", lightSettingsFile);

	lighting.setName("Lighting");
	lighting.add(lightPos.set("Light Position", ofPoint(0, 0, 0), ofPoint(-500, -500, -500), ofPoint(500, 500, 500)));
	lighting.add(m_ambient.set("Material Ambient", ofColor(255)));
	lighting.add(m_diffuse.set("Material Diffuse", ofColor(255)));
	lighting.add(m_specular.set("Material Specular", ofColor(255)));
	lighting.add(shininess.set("Material Shininess", 1.0, 0.0, 50.0));
	lighting.add(l_ambient.set("Light Ambient", ofColor(255)));
	lighting.add(l_diffuse.set("Light Diffuse", ofColor(255)));
	lighting.add(l_specular.set("Light Specular", ofColor(255)));
	lighting.add(blur.set("Rim Glow", 1, 0, 20));

	lightGui.add(lighting);

	lightGui.loadFromFile(lightSettingsFile);

	string supershapeSettingsFile = "settings/supershape.xml";

	supershapeGui.setup("Supershape", supershapeSettingsFile);

	supershapeGroup1.setName("Shape 1");

	supershapeGroup1.add(m1.set("M", 1, 0, 20));
	supershapeGroup1.add(n11.set("N1", 1, 0, 100));
	supershapeGroup1.add(n21.set("N2", 1, 0, 100));
	supershapeGroup1.add(n31.set("N3", 1, 0, 100));
	supershapeGroup1.add(a1.set("A", 1, 0, 2));
	supershapeGroup1.add(b1.set("B", 1, 0, 1));

	supershapeGroup2.setName("Shape 2");

	supershapeGroup2.add(m2.set("M", 1, 0, 20));
	supershapeGroup2.add(n12.set("N1", 1, 0, 100));
	supershapeGroup2.add(n22.set("N2", 1, 0, 100));
	supershapeGroup2.add(n32.set("N3", 1, 0, 100));
	supershapeGroup2.add(a2.set("A", 1, 0, 2));
	supershapeGroup2.add(b2.set("B", 1, 0, 1));

	supershapeGui.add(supershapeGroup1);
	supershapeGui.add(supershapeGroup2);
	supershapeGui.add(drawNormals.set("Draw Normals", false));
	supershapeGui.add(scale.set("Scale", 20.0, 1.0, 100.0));
    supershapeGui.add(speed.set("Speed", 1.0, 0.0, 2.0));
    supershapeGui.add(noiseScale.set("Noise Scale", 1.0, 0.0, 2.0));
    supershapeGui.add(noiseResolution.set("Noise Resolution", 1.0, 0.0, 20.0));

	supershapeGui.setPosition(100 + supershapeGui.getWidth() - 10, 10);

	supershapeGui.loadFromFile(supershapeSettingsFile);

	light.load("shaders/light");
	rimLight.load("shaders/light.vert", "shaders/rimLight.frag");
	blurShader.load("shaders/blur");

	ofBackground(229.5);

	rimBuffer.allocate(ofGetWidth(), ofGetHeight());
	objectBuffer.allocate(ofGetWidth(), ofGetHeight());
	blurBuffer.allocate(ofGetWidth(), ofGetHeight());
    
    ofHttpResponse resp = ofLoadURL("http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=e69f027f111c4b9f3d7c5cef5da1c8a7");
    cout<<resp.data<<endl;
    
<<<<<<< Updated upstream
=======
    ofxNestedFileLoader loader;
    imagePaths = loader.load("sunrisekingdomimages");
    
    img.load(imagePaths[0]);
    imgIndex = 0;
    
>>>>>>> Stashed changes
    ofEnableAntiAliasing();
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateFaceNormal(ofVec3f A, ofVec3f B, ofVec3f C) {
	ofVec3f EB, EC;
	EB = B - A;
	EC = C - A;
	return EB.crossed(EC);
}

//--------------------------------------------------------------
void ofApp::update(){
<<<<<<< Updated upstream


    /*
	float rollPercent = ofMap(cam.getRoll(), -90, 90, 0, 20);
	float pitchPercent = ofMap(cam.getPitch(), -180, 180, 1, 100);

	m2 = ofLerp(m2, rollPercent, 0.3);
	n22 = ofLerp(n22, pitchPercent, 0.3);
     */
=======
    
>>>>>>> Stashed changes
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	ofBackground(0);

	objectBuffer.begin();
	ofClear(255, 255, 255, 0);
	ofEnableDepthTest();
	cam.begin();
	light.begin();
	light.setUniform3f("l_position", lightPos);
	light.setUniform3f("l_ambient", scaleColorToUniform(l_ambient));
	light.setUniform3f("l_diffuse", scaleColorToUniform(l_diffuse));
	light.setUniform3f("l_specular", scaleColorToUniform(l_specular));
	light.setUniform3f("m_ambient", scaleColorToUniform(m_ambient));
	light.setUniform3f("m_diffuse", scaleColorToUniform(m_diffuse));
	light.setUniform3f("m_specular", scaleColorToUniform(m_specular));

	light.setUniform1f("m1", m1);
	light.setUniform1f("n11", n11);
	light.setUniform1f("n21", n21);
	light.setUniform1f("n31", n31);
	light.setUniform1f("a1", a1);
	light.setUniform1f("b1", b1);

	light.setUniform1f("m2", m2);
	light.setUniform1f("n12", n12);
	light.setUniform1f("n22", n22);
	light.setUniform1f("n32", n32);
	light.setUniform1f("a2", a2);
	light.setUniform1f("b2", b2);

	light.setUniform1f("shininess", shininess);
	light.setUniform1f("scale", scale);
    light.setUniform1f("time", ofGetElapsedTimef());
    light.setUniform1f("speed", speed);
    light.setUniform1f("noiseScale", noiseScale);
    light.setUniform1f("noiseResolution", noiseResolution);

	light.setUniform1f("stepSize", NUM_STEPS);

	mesh.draw();

	light.end();

	cam.end();
	ofDisableDepthTest();
	objectBuffer.end();

	ofDisableDepthTest();
	ofSetColor(255);
    
<<<<<<< Updated upstream
    blurShader.begin();
    blurShader.setUniform2f("resolution", objectBuffer.getWidth(), objectBuffer.getHeight());
    blurShader.setUniformTexture("inputTexture", objectBuffer.getTexture(), 0);
    ofDrawRectangle(0, 0, objectBuffer.getWidth(), objectBuffer.getHeight());
	//objectBuffer.draw(0, 0);
    
    blurShader.end();
=======
   //  blurShader.begin();
    //blurShader.setUniform2f("resolution", objectBuffer.getWidth(), objectBuffer.getHeight());
   // blurShader.setUniformTexture("inputTexture", objectBuffer.getTexture(), 0);
    //ofDrawRectangle(0, 0, objectBuffer.getWidth(), objectBuffer.getHeight());
    objectBuffer.draw(0, 0);
    //blurShader.end();
>>>>>>> Stashed changes

	if(showGui) {
		lightGui.draw();
		supershapeGui.draw();
        img.draw(ofGetWidth() - img.getWidth()/10, 0, img.getWidth()/10, img.getHeight()/10);
        drawColors(cols, ofGetWidth() - img.getWidth()/10, img.getHeight()/10);
	}
}

//--------------------------------------------------------------
void ofApp::drawColors(vector<ofColor> cols, int _x, int _y) {
    float x = _x;
    float y = _y;
    for(int i = 0; i < cols.size(); i++) {
        ofSetColor(cols[i]);
        ofDrawRectangle(x, y, 20, 20);
        x += 20;
        if(x > ofGetWidth()) {
            x = _x;
            y += 20;
        }
    }
}

//--------------------------------------------------------------
ofVec3f ofApp::scaleColorToUniform(ofColor col) {
	float r = ofMap(col.r, 0, 255, 0, 1);
	float g = ofMap(col.g, 0, 255, 0, 1);
	float b = ofMap(col.b, 0, 255, 0, 1);
	return ofVec3f(r, g, b);
}

//--------------------------------------------------------------
ofVec3f ofApp::cartesianToSpherical(ofVec3f point) {
	float r = point.length();
	float theta = acos(point.z / r);
	float phi = atan(point.y / point.x);
	return ofVec3f(theta, phi, r);
}

//--------------------------------------------------------------
ofVec3f ofApp::getPoint(float theta, float phi) {
	float r1 = supershape(theta, m1, n11, n21, n31);//supershape(theta, 7, 0.2, 1.7, 1.7);
	float r2 = supershape(phi, m2, n12, n22, n32);// supershape(phi, 7, 2, 1.7, 1.7);
	float x = r1*cos(theta)*r2*cos(phi);
	float y = r1*sin(theta)*r2*cos(phi);
	float z = r2*sin(phi);

	return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
float ofApp::supershape(float theta, float m, float n1, float n2, float n3) {
	float a, b;
	a = b = 1;

	float r = pow(pow(abs(1 / a*cos(m * theta / 4)), n2) + pow(abs(1 / b*sin(m*theta / 4)), n3), -1 / n1);

	return r;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 'g')
		showGui = !showGui;
    if(key == OF_KEY_RIGHT) {
        imgIndex++;
        imgIndex %= imagePaths.size();
        img.load(imagePaths[imgIndex]);
        cols = getColorsFromImage(img);
        applyColorsToSupershape(cols);
    }
    if(key == OF_KEY_LEFT) {
        imgIndex--;
        if(imgIndex < 0)
            imgIndex = imagePaths.size() - 1;
        img.load(imagePaths[imgIndex]);
        cols = getColorsFromImage(img);
        applyColorsToSupershape(cols);
    }
}

//--------------------------------------------------------------
vector<ofColor> ofApp::getColorsFromImage(ofImage img) {
    vector<pair<ofColor, int>> colorBins;
    
    float distTolerance = 30.0;
    
    for (int x = 0; x < img.getWidth(); x += img.getWidth()/100) {
        for (int y = 0; y < img.getHeight(); y += img.getHeight() / 100) {
            ofColor col = img.getColor(x, y);
            bool binned = false;
            for (auto it = colorBins.begin(); it != colorBins.end(); it++) {
                if (getColorDistance(col, it->first) < distTolerance) {
                    it->first = getBlendedColor(it->first, it->second, col, 1);
                    it->second++;
                    binned = true;
                    break;
                }
            }
            if (!binned) {
                // Create new bin
                pair<ofColor, int> p;
                p.first = col;
                p.second = 1;
                colorBins.push_back(p);
            }
        }
    }
    
    vector<ofColor> cols;
    for(int i = 0; i < colorBins.size(); i++) {
        cols.push_back(colorBins[i].first);
    }
    return cols;
}

void ofApp::applyColorsToSupershape(vector<ofColor> cols) {
    vector<ofColor> culledColors;
    float threshold = 150;
    for(int i = 0; i < cols.size(); i++) {
        if(cols[i].r > threshold || cols[i].g > threshold || cols[i].b > threshold) {
            culledColors.push_back(cols[i]);
        }
    }
    int i = 0;
    m_ambient = culledColors[i];
    i += int(culledColors.size() / 6);
    m_diffuse = culledColors[i];
    i += int(culledColors.size() / 6);
    m_specular = culledColors[i];
    i += int(culledColors.size() / 6);
    l_ambient = culledColors[i];
    i += int(culledColors.size() / 6);
    l_diffuse = culledColors[i];
    i += int(culledColors.size() / 6);
    l_specular = culledColors[i];

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
