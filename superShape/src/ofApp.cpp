#include "ofApp.h"

#define NUM_STEPS 500

//--------------------------------------------------------------
void ofApp::setup(){

	cam.setPosition(0, 0, 5);

	mesh = mesh.sphere(1, NUM_STEPS);

	string lightSettingsFile = "settings/lighting.xml";
	lightGui.setup("Lighting", lightSettingsFile);

	lighting.setName("Lighting");
	lighting.add(lightPos.set("Light Position", ofPoint(0, 0, 0), ofPoint(-200, -200, -200), ofPoint(200, 200, 200)));
	lighting.add(m_ambient.set("Material Ambient", ofColor(255)));
	lighting.add(m_diffuse.set("Material Diffuse", ofColor(255)));
	lighting.add(m_specular.set("Material Specular", ofColor(255)));
	lighting.add(shininess.set("Material Shininess", 1.0, 0.0, 50.0));
	//lighting.add(m_emissive.set("Material Emissive", ofColor(255)));
	lighting.add(l_ambient.set("Light Ambient", ofColor(255)));
	lighting.add(l_diffuse.set("Light Diffuse", ofColor(255)));
	lighting.add(l_specular.set("Light Specular", ofColor(255)));

	lightGui.add(lighting);

	lightGui.loadFromFile(lightSettingsFile);

	string supershapeSettingsFile = "settings/supershape.xml";

	supershapeGui.setup("Supershape", supershapeSettingsFile);

	supershapeGroup1.add(m1.set("M", 1, 0, 20));
	supershapeGroup1.add(n11.set("N1", 1, 0, 100));
	supershapeGroup1.add(n21.set("N2", 1, 0, 100));
	supershapeGroup1.add(n31.set("N3", 1, 0, 100));
	supershapeGroup1.add(a1.set("A", 1, 0, 2));
	supershapeGroup1.add(b1.set("B", 1, 0, 1));


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

	supershapeGui.setPosition(ofGetWidth() - supershapeGui.getWidth() - 10, 10);

	supershapeGui.loadFromFile(supershapeSettingsFile);

	light.load("shaders/light");

	ofBackground(0);
}

ofVec3f ofApp::calculateFaceNormal(ofVec3f A, ofVec3f B, ofVec3f C) {
	ofVec3f EB, EC;
	EB = B - A;
	EC = C - A;
	return EB.crossed(EC);
}

//--------------------------------------------------------------
void ofApp::update(){

	float rollPercent = ofMap(cam.getRoll(), -90, 90, 0, 20);
	float pitchPercent = ofMap(cam.getPitch(), -180, 180, 0, 20);

	m1 = ofLerp(m1, rollPercent, 0.1);
	m2 = ofLerp(m2, pitchPercent, 0.1);
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofEnableDepthTest();
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

	ofSetColor(255);
	mesh.draw();

	light.end();

	ofDrawAxis(100);

	cam.end();

	ofDisableDepthTest();
	lightGui.draw();
	supershapeGui.draw();
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
