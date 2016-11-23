#include "ofApp.h"

#define NUM_STEPS 100

//--------------------------------------------------------------
void ofApp::setup(){

	cam.setPosition(0, 0, 5);
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
			mesh.addIndex((i + j*NUM_STEPS));               
			mesh.addIndex(((i + 1) + j*NUM_STEPS));
		}
	}

	mesh.setMode(OF_PRIMITIVE_LINE_LOOP);

	string lightSettingsFile = "settings/lighting.xml";
	lightGui.setup("Lighting", lightSettingsFile);

	lighting.setName("Lighting");
	lighting.add(lightPos.set("Light Position", ofPoint(100, 100, 0), ofPoint(-200, -200, -200), ofPoint(200, 200, 200)));
	lighting.add(m_ambient.set("Material Ambient", ofColor(255)));
	lighting.add(m_diffuse.set("Material Diffuse", ofColor(255)));
	lighting.add(m_specular.set("Material Specular", ofColor(255)));
	lighting.add(shininess.set("Material Shininess", 1.0, 0.0, 1.0));
	lighting.add(m_emissive.set("Material Emissive", ofColor(255)));
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

	supershapeGroup2.add(m2.set("M", 1, 0, 20));
	supershapeGroup2.add(n12.set("N1", 1, 0, 100));
	supershapeGroup2.add(n22.set("N2", 1, 0, 100));
	supershapeGroup2.add(n32.set("N3", 1, 0, 100));

	supershapeGui.add(supershapeGroup1);
	supershapeGui.add(supershapeGroup2);
	supershapeGui.add(step.set("Step", 0, 0, NUM_STEPS*NUM_STEPS));

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
	mesh.clear();
	for (int i = 0; i < NUM_STEPS+1; i++) {
		float theta = ofMap(i, 0, NUM_STEPS-1, -PI, PI);
		float thetaPlus = ofMap(i + 1, 0, NUM_STEPS - 1, -PI, PI);
		float thetaMinus = ofMap(i - 1, 0, NUM_STEPS - 1, -PI, PI);
		for (int j = 0; j < NUM_STEPS+1; j++) {
			float phi = ofMap(j, 0, NUM_STEPS-1, -PI / 2, PI / 2);
			float phiPlus = ofMap(j + 1, 0, NUM_STEPS - 1, -PI / 2, PI / 2);
			float phiMinus = ofMap(j - 1, 0, NUM_STEPS - 1, -PI / 2, PI / 2);

			ofVec3f point = getPoint(theta, phi);
			ofVec3f pointPlusTheta = getPoint(thetaPlus, phi);
			ofVec3f pointPlusPhi = getPoint(theta, phiPlus);
			ofVec3f pointMinusTheta = getPoint(thetaMinus, phi);
			ofVec3f pointMinusPhi = getPoint(theta, phiMinus);
			ofVec3f pointPlusThetaMinusPhi = getPoint(thetaPlus, phiMinus);
			ofVec3f pointMinusThetaPlusPhi = getPoint(thetaMinus, phiPlus);

			ofVec3f faceNorm1 = calculateFaceNormal(point, pointPlusTheta, pointPlusThetaMinusPhi);
			ofVec3f faceNorm2 = calculateFaceNormal(point, pointMinusPhi, pointPlusThetaMinusPhi);
			ofVec3f faceNorm3 = calculateFaceNormal(point, pointMinusTheta, pointMinusPhi);
			ofVec3f faceNorm4 = calculateFaceNormal(point, pointMinusTheta, pointMinusThetaPlusPhi);
			ofVec3f faceNorm5 = calculateFaceNormal(point, pointMinusThetaPlusPhi, pointPlusPhi);
			ofVec3f faceNorm6 = calculateFaceNormal(point, pointPlusPhi, pointPlusTheta);

			ofVec3f norm = faceNorm1 + faceNorm2 + faceNorm3 + faceNorm4 + faceNorm5 + faceNorm6;
			norm.normalize();
			norm *= -1;
			mesh.addVertex(50  * point);
			mesh.addNormal(norm);
		}
	}

	int index1, index2, index3;
	for (int y = 0; y<NUM_STEPS; y++) {
		for (int x = 0; x<NUM_STEPS; x++) {
			index1 = x + y*NUM_STEPS;
			index2 = (x + 1) + y*NUM_STEPS;
			index3 = x + (y + 1)*NUM_STEPS;
			mesh.addIndex(index1);               
			mesh.addIndex(index2);           
			mesh.addIndex(index3);
			//A = mesh.getVertex(index1);
			//B = mesh.getVertex(index2);
			//C = mesh.getVertex(index3);
			//EB = B - A;
			//EC = C - A;
			//N = EB.cross(EC).normalize();
			//mesh.addNormal(N);

			index1 = (x + 1) + y*NUM_STEPS;
			index2 = (x + 1) + (y + 1)*NUM_STEPS;
			index3 = x + (y + 1)*NUM_STEPS;
			mesh.addIndex(index1);
			mesh.addIndex(index2);
			mesh.addIndex(index3);
	/*		A = mesh.getVertex(index1);
			B = mesh.getVertex(index2);
			C = mesh.getVertex(index3);
			EB = B - A;
			EC = C - A;
			N = EB.cross(EC).normalize();
			mesh.addNormal(N);*/
		}
	}
			//ofVec3f A = mesh.getVertex(index1);
			//ofVec3f B = mesh.getVertex(index2);
			//ofVec3f C = mesh.getVertex(lastIndex);
			//ofVec3f E1 = B - A;
			//ofVec3f E2 = C - A;
			//ofVec3f N = E1.getCrossed(E2);
			//N.normalize();
			//mesh.addNormal(N);

	//for(int i = NUM_STEPS; i > 0; i--) {
	//	for (int j = NUM_STEPS; j > 0; j--) {
	//		int index1 = (i + j*NUM_STEPS);
	//		int index2 = (i - 1) + j*NUM_STEPS;
	//		int index3 = i + (j - 1)*NUM_STEPS;
	//		mesh.addIndex(index1);
	//		mesh.addIndex(index2);
	//		mesh.addIndex(index3);
	//		ofColor col = ofColor(0, 255, 0);
	//		mesh.setColor(index1, col);
	//		mesh.setColor(index2, col);
	//		mesh.setColor(index3, col);
	//	}
	//}

	float rollPercent = ofMap(cam.getRoll(), -90, 90, 0, 20);
	float pitchPercent = ofMap(cam.getPitch(), -180, 180, 0, 20);

	//m1 = ofLerp(m1, rollPercent, 0.1);
	//m2 = ofLerp(m2, pitchPercent, 0.1);


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
	light.setUniform3f("m_emissize", scaleColorToUniform(m_emissive));

	light.setUniform1f("shininess", shininess);
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	//vector<ofPoint> normals = mesh.getNormals();
	//ofSetColor(255, 0, 0);
	//for (int i = 0; i < normals.size(); i++) {
	//	ofDrawLine(mesh.getVertex(i), mesh.getVertex(i) + normals[i] * 2);
	//}
	ofSetColor(255);
	mesh.draw();
	light.end();
	mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
	//ofSetColor(255);
	//mesh.draw();
	ofDrawAxis(100);
	//ofTranslate(lightPos);
	//ofDrawSphere(5);
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
