#include "ofApp.h"

#define NUM_STEPS 20

//--------------------------------------------------------------
void ofApp::setup(){

	cam.setPosition(0, 0, 20);
	float r = 50;

	for (int i = 0; i < NUM_STEPS; i++) {
		float theta = ofMap(i, 0, NUM_STEPS, -PI, PI);
		for (int j = 0; j < NUM_STEPS; j++) {
			float phi = ofMap(j, 0, NUM_STEPS, -PI / 2, PI / 2);
			mesh.addVertex(r * getPoint(theta, phi));
		}
	}
	for (int i = 0; i < NUM_STEPS - 1; i++) {
		for (int j = 0; j < NUM_STEPS - 1; j++) {
			mesh.addIndex((i + j*NUM_STEPS));               // 0
			mesh.addIndex(((i + 1) + j*NUM_STEPS));           // 1
			mesh.addIndex((i + (j + 1)*NUM_STEPS));           // 10

			mesh.addIndex(((i + 1) + j*NUM_STEPS));           // 1
			mesh.addIndex(((i + 1) + (j + 1)*NUM_STEPS));       // 11
			mesh.addIndex((i + (j + 1)*NUM_STEPS));           // 10
		}
	}

	mesh.setMode(OF_PRIMITIVE_LINE_LOOP);

	gui.setup("GUI", "settings/settings.xml");

	lighting.setName("Lighting");
	lighting.add(lightPos.set("Light Position", ofPoint(100, 100, 0), ofPoint(-200, -200, -200), ofPoint(200, 200, 200)));
	lighting.add(m_ambient.set("Material Ambient", ofColor(255)));
	lighting.add(m_diffuse.set("Material Diffuse", ofColor(255)));
	lighting.add(m_specular.set("Material Specular", ofColor(255)));
	lighting.add(shininess.set("Material Shininess", 1.0, 0.0, 1.0));
	lighting.add(l_ambient.set("Light Ambient", ofColor(255)));
	lighting.add(l_diffuse.set("Light Diffuse", ofColor(255)));
	lighting.add(l_specular.set("Light Specular", ofColor(255)));

	mat.setAmbientColor(m_ambient.get());
	mat.setDiffuseColor(m_diffuse.get());
	mat.setSpecularColor(m_specular.get());

	supershapeGroup.add(m.set("M", 1, 0, 5));
	supershapeGroup.add(n1.set("N1", 1, 0, 5));
	supershapeGroup.add(n2.set("N2", 1, 0, 5));
	supershapeGroup.add(n3.set("N3", 1, 0, 5));

	//gui.add(lighting);

	gui.add(supershapeGroup);

	//ofEnableLighting();

	//light.load("shaders/light");

	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
	mesh.clear();
	for (int i = 0; i < NUM_STEPS; i++) {
		float theta = ofMap(i, 0, NUM_STEPS, -PI, PI);
		for (int j = 0; j < NUM_STEPS; j++) {
			float phi = ofMap(j, 0, NUM_STEPS, -PI / 2, PI / 2);
			mesh.addVertex(50 * getPoint(theta, phi));
			mesh.addColor(ofColor(255));
		}
	}
	//for (int i = 0; i < NUM_STEPS - 1; i++) {
	//	for (int j = 0; j < NUM_STEPS - 1; j++) {
	//		mesh.addIndex((i + j*NUM_STEPS));               // 0
	//		mesh.addIndex(((i + 1) + j*NUM_STEPS));           // 1
	//		mesh.addIndex((i + (j + 1)*NUM_STEPS));           // 10

	//		mesh.addIndex(((i + 1) + j*NUM_STEPS));           // 1
	//		mesh.addIndex(((i + 1) + (j + 1)*NUM_STEPS));       // 11
	//		mesh.addIndex((i + (j + 1)*NUM_STEPS));           // 10
	//	}
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	cam.begin();
	//light.enable();
	//mat.begin();
	//light.begin();
	//light.setUniform3f("l_position", lightPos);
	//light.setUniform3f("l_ambient", scaleColorToUniform(l_ambient));
	//light.setUniform3f("l_diffuse", scaleColorToUniform(l_diffuse));
	//light.setUniform3f("l_specular", scaleColorToUniform(l_specular));
	//light.setUniform3f("m_ambient", scaleColorToUniform(m_ambient));
	//light.setUniform3f("m_diffuse", scaleColorToUniform(m_diffuse));
	//light.setUniform3f("m_specular", scaleColorToUniform(m_specular));
	//light.setUniform1f("shininess", shininess);
	//ofDrawSphere(20);
	mesh.draw();
	//mat.end();
	//light.disable();
	//light.draw();
	//light.end();
	//ofTranslate(lightPos);
	//ofDrawSphere(5);
	cam.end();

	gui.draw();
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
	float r1 = supershape(theta, m, n1, n2, n3);//supershape(theta, 7, 0.2, 1.7, 1.7);
	float r2 = supershape(phi, m, n1, n2, n3);// supershape(phi, 7, 2, 1.7, 1.7);
	float x = r1*supershape(theta)*cos(theta)*r2*cos(phi);
	float y = r1*sin(theta)*r2*cos(phi);
	float z = r2*sin(phi);

	return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
float ofApp::supershape(float theta, float m, float n1, float n2, float n3) {
	float a, b;
	a = b = 1;

	float r = pow(pow(w1 / a*cos(m * theta / 4)), n2) + pow(abs(1 / b*sin(m*theta / 4)), n3), -1 / n1);

	return r;
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
