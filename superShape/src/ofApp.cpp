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
	lighting.add(lightPos.set("Light Position", ofPoint(1000, 0, 500), ofPoint(-2000, -2000, -2000), ofPoint(2000, 2000, 2000)));
    lighting.add(moonPos.set("Moon Position", ofPoint(1000, 0, 500), ofPoint(-2000, -2000, -2000), ofPoint(2000, 2000, 2000)));
	lighting.add(m_ambient.set("Material Ambient", ofColor(255)));
	lighting.add(m_diffuse.set("Material Diffuse", ofColor(255)));
	lighting.add(m_specular.set("Material Specular", ofColor(255)));
	lighting.add(shininess.set("Material Shininess", 1.0, 0.0, 50.0));
	lighting.add(l_ambient.set("Light Ambient", ofColor(255)));
	lighting.add(l_diffuse.set("Light Diffuse", ofColor(255)));
	lighting.add(l_specular.set("Light Specular", ofColor(255)));
    lighting.add(moon_ambient.set("Moon Ambient", ofColor(255)));
    lighting.add(moon_diffuse.set("Moon Diffuse", ofColor(255)));
    lighting.add(moon_specular.set("Moon Specular", ofColor(255)));
	lighting.add(blur.set("Rim Glow", 1, 0, 20));

	lightGui.add(lighting);

	lightGui.loadFromFile(lightSettingsFile);

	string supershapeSettingsFile = "settings/supershape.xml";

	supershapeGui.setup("Supershape", supershapeSettingsFile);
    supershapeGui.add(bufferSize.set("Buffer Size", ofGetHeight(), 0.0, ofGetHeight()));

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
	supershapeGui.add(scale.set("Scale", 20.0, 1.0, 200.0));
    supershapeGui.add(speed.set("Speed", 1.0, 0.0, 2.0));
    supershapeGui.add(noiseScale.set("Noise Scale", 1.0, 0.0, 2.0));
    supershapeGui.add(noiseResolution.set("Noise Resolution", 1.0, 0.0, 20.0));

	supershapeGui.setPosition(100 + supershapeGui.getWidth() - 10, 10);

	supershapeGui.loadFromFile(supershapeSettingsFile);

	light.load("shaders/light");
	rimLight.load("shaders/light.vert", "shaders/rimLight.frag");
	blurShader.load("shaders/blur");

	ofBackground(229.5);
    
    ofxNestedFileLoader loader;
//    imagePaths = loader.load("sunrisekingdomimages");
    imagePaths = loader.load("Images");

    weather.updateWeather();
    
    img.load(imagePaths[0]);
    imgIndex = 0;
    
    ofEnableAntiAliasing();
	rimBuffer.allocate(ofGetHeight(), ofGetHeight());
	objectBuffer.allocate(ofGetHeight(), ofGetHeight());
    blurBuffer.allocate(bufferSize, bufferSize);
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
    weather.updateWeatherWithInterval();
    ofVec3f lPos = lightPos.get();
    lPos = cartesianToSpherical(lPos);
    lPos.y = ofMap(ofGetUnixTime(), weather.sunrise, weather.sunset, PI, 0);
    lPos = sphericalToCartesian(lPos);
    
    ofVec3f mPos = lPos;
    mPos.x *= -1;
    mPos.y *= -1;
    
    moonPos.set(mPos);

    lightPos.set(lPos);
    
    if(selectedCols.size())
        applyColorsToSupershape(selectedCols);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	ofBackground(0);

	objectBuffer.begin();
	ofClear(255, 255, 255, 0);
	ofEnableDepthTest();
	cam.begin();
	light.begin();
    light.setUniform3f("c_position", cam.getPosition());
	light.setUniform3f("l_position", lightPos);
    light.setUniform3f("moon_position", moonPos);
	light.setUniform3f("l_ambient", scaleColorToUniform(l_ambient));
	light.setUniform3f("l_diffuse", scaleColorToUniform(l_diffuse));
	light.setUniform3f("l_specular", scaleColorToUniform(l_specular));
	light.setUniform3f("m_ambient", scaleColorToUniform(m_ambient));
	light.setUniform3f("m_diffuse", scaleColorToUniform(m_diffuse));
	light.setUniform3f("m_specular", scaleColorToUniform(m_specular));
    light.setUniform3f("moon_ambient", scaleColorToUniform(moon_ambient));
    light.setUniform3f("moon_diffuse", scaleColorToUniform(moon_diffuse));
    light.setUniform3f("moon_specular", scaleColorToUniform(moon_specular));

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
    
    ofPushMatrix();
    ofTranslate(cam.getPosition());

    ofVec3f v = lightPos.get();// + cam.getPosition();

    ofDrawSphere(v, 20);
    ofPopMatrix();

	cam.end();
	ofDisableDepthTest();
	objectBuffer.end();

	ofDisableDepthTest();
	ofSetColor(255);
    
   //  blurShader.begin();
    //blurShader.setUniform2f("resolution", objectBuffer.getWidth(), objectBuffer.getHeight());
   // blurShader.setUniformTexture("inputTexture", objectBuffer.getTexture(), 0);
    //ofDrawRectangle(0, 0, objectBuffer.getWidth(), objectBuffer.getHeight());
    //objectBuffer.draw(0, 0);
    //blurShader.end();

	//blurBufferX.begin();
	//ofClear(255, 255, 255, 0);
	//blurX.begin();
	//blurX.setUniform1f("blurAmnt", blur);
	//rimBuffer.draw(0, 0);
	//blurX.end();
	//blurBufferX.end();

	//blurBufferY.begin();
	//ofClear(255, 255, 255, 0);
	//blurY.begin();
	//blurY.setUniform1f("blurAmnt", blur);
	//blurBufferX.draw(0, 0);
	//blurY.end();
	//blurBufferY.end();

	//combine.begin();
	//combine.setUniformTexture("u_Object", objectBuffer.getTexture(), 0);
	//combine.setUniformTexture("u_Rim", blurBufferY.getTexture(), 1);
	//combine.setUniform2f("u_Res", ofVec2f(ofGetWidth(), ofGetHeight()));
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//combine.end();
/*
    blurBuffer.begin();
    ofClear(0);
    blurShader.begin();
    blurShader.setUniformTexture("inputTexture", objectBuffer, 0);
    blurShader.setUniform2f("resolution", bufferSize, bufferSize);
    blurShader.end();
    blurBuffer.end();
    */
    objectBuffer.draw(0, 0, bufferSize, bufferSize);

	//blurBufferY.draw(0, 0);
	//objectBuffer.draw(0, 0);
	if(showGui) {
        ofSetColor(255);
		lightGui.draw();
		supershapeGui.draw();
        img.draw(ofGetWidth()/2 - img.getWidth()/10/2, 0, img.getWidth()/10, img.getHeight()/10);
        drawColors(cols, ofGetWidth()/2 - img.getWidth()/10/2, img.getHeight()/10, ofGetWidth()/2 + img.getWidth()/10/2);
	}
}

//--------------------------------------------------------------
void ofApp::drawColors(vector<ofColor> cols, int _x, int _y, float limit) {
    float x = _x;
    float y = _y;
    for(int i = 0; i < cols.size(); i++) {
        ofSetColor(cols[i]);
        ofDrawRectangle(x, y, 20, 20);
        x += 20;
        if(x+20 > limit) {
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
    float theta = atan2(point.y , point.x);
	float phi = acos(point.z / r);
	return ofVec3f(r, theta, phi);
}

//--------------------------------------------------------------
ofVec3f ofApp::sphericalToCartesian(ofVec3f point) {
    float r = point.x;
    float theta = point.y;
    float phi = point.z;
    float x = r * cos(theta) * sin(phi);
    float y = r * sin(theta) * sin(phi);
    float z = r * cos(phi);
    return ofVec3f(x, y, z);
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
    }
    if(key == OF_KEY_LEFT) {
        imgIndex--;
        if(imgIndex < 0)
            imgIndex = imagePaths.size() - 1;
        img.load(imagePaths[imgIndex]);
        cols = getColorsFromImage(img);
    }
}

bool compareHue(ofColor col1, ofColor col2) {
    int h1 = col1.getHue();
    int h2 = col2.getHue();
    return h1 < h2;
}

bool compareNumColors(pair<ofColor, int> col1, pair<ofColor, int> col2) {
    return col1.second < col2.second;
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
    

    std::sort(colorBins.begin(), colorBins.end(), compareNumColors);
    
    vector<ofColor> cols;
    for(int i = 0; i < colorBins.size(); i++) {
        cols.push_back(colorBins[i].first);
    }

    return cols;
}

void ofApp::applyColorsToSupershape(vector<ofColor> cols) {
    
    int i = 0;
    /*
    m_ambient = m_ambient.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
    
    m_specular = m_specular.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
    
    m_diffuse = m_diffuse.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
     */
    
    moon_ambient = moon_ambient.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();

    moon_diffuse = moon_diffuse.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
    
    moon_specular = moon_specular.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
    
    l_ambient = l_ambient.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();
    
    l_diffuse = l_diffuse.get().getLerped(cols[i], 0.1);
    i++;
    i %= cols.size();

    l_specular = l_specular.get().getLerped(cols[i], 0.1);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' ') {
        vector<ofColor> culledColors;
        float threshold = 150;
        for(int i = 0; i < cols.size(); i++) {
            if(cols[i].r > threshold || cols[i].g > threshold || cols[i].b > threshold) {
                culledColors.push_back(cols[i]);
            }
        }
        selectedCols.clear();
        for(int i = 0; i < 9; i++) {
            selectedCols.push_back(culledColors[int(ofRandom(culledColors.size()))]);
        }
    }
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
