#version 120
#extension GL_EXT_gpu_shader4 : enable

#define PI 3.1415926

varying vec3 normal;
varying vec3 position;

uniform float stepSize;

uniform float scale;
uniform float m1 = 0;
uniform float n11 = 0.2f;
uniform float n21 = 1.7f;
uniform float n31 = 1.7f;
uniform float a1 = 1.0f;
uniform float b1 = 1.0f;

uniform float m2 = 0;
uniform float n12 = 0.2f;
uniform float n22 = 1.7f;
uniform float n32 = 1.7f;
uniform float a2 = 1.0f;
uniform float b2 = 1.0f;

//--------------------------------------------------------------
float supershape(float theta, float m, float n1, float n2, float n3 ,float a, float b) {

	float r = pow(pow(abs(1 / a*cos(m * theta / 4)), n2) + pow(abs(1 / b*sin(m*theta / 4)), n3), -1 / n1);

	return r;
}

vec3 calculateFaceNormal(vec3 A, vec3 B, vec3 C) {
	vec3 EB, EC;
	EB = B - A;
	EC = C - A;
	return cross(EB, EC);
}

//--------------------------------------------------------------
vec3 getPoint(float theta, float phi) {
	float r1 = supershape(theta, m1, n11, n21, n31, a1, b1);
	float r2 = supershape(phi, m2, n12, n22, n32, a2, b2);
	float x = r1*cos(theta)*r2*cos(phi);
	float y = r1*sin(theta)*r2*cos(phi);
	float z = r2*sin(phi);

	return vec3(x, y, z);
}

float map(float val, float inMin, float inMax, float outMin, float outMax) {
	return outMin + (val - inMin) * (outMax - outMin) / (inMax - inMin);
}

void main(){
	// get original vertex postion
	vec4 pos = gl_Vertex;

	float r = 1.0;
	float theta = atan(pos.y, pos.x);// outputs of atan and acos are in the wrong range so we need to slide them back in to place!
	float phi = acos(pos.z / r);
	theta -= PI;
	phi -= PI / 2;

	float thetaPlus = theta + 1.0f/stepSize;
	float phiPlus = phi + 1.0f/ stepSize;
	float thetaMinus = theta - 1.0f/ stepSize;
	float phiMinus = phi - 1.0f/ stepSize;

	vec3 point = getPoint(theta, phi);
	vec3 pointPlusTheta = getPoint(thetaPlus, phi);
	vec3 pointPlusPhi = getPoint(theta, phiPlus);
	vec3 pointMinusTheta = getPoint(thetaMinus, phi);
	vec3 pointMinusPhi = getPoint(theta, phiMinus);
	vec3 pointPlusThetaMinusPhi = getPoint(thetaPlus, phiMinus);
	vec3 pointMinusThetaPlusPhi = getPoint(thetaMinus, phiPlus);

	vec3 faceNorm1 = calculateFaceNormal(point, pointMinusPhi, pointPlusThetaMinusPhi);
	vec3 faceNorm2 = calculateFaceNormal(point, pointPlusThetaMinusPhi, pointPlusTheta);
	vec3 faceNorm3 = calculateFaceNormal(point, pointPlusTheta, pointPlusPhi);
	vec3 faceNorm4 = calculateFaceNormal(point, pointPlusPhi, pointMinusThetaPlusPhi);
	vec3 faceNorm5 = calculateFaceNormal(point, pointMinusThetaPlusPhi, pointMinusTheta);
	vec3 faceNorm6 = calculateFaceNormal(point, pointMinusTheta, pointMinusPhi);

	vec3 norm = faceNorm1 + faceNorm2 + faceNorm3 + faceNorm4 + faceNorm5 + faceNorm6;
	norm *= -1;

	pos.xyz = point * scale;

	pos = gl_ModelViewMatrix * pos;

	position = pos.xyz;

	pos =  gl_ProjectionMatrix * pos;

	normal = vec3(gl_NormalMatrix * norm);

	gl_Position = pos;//gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
