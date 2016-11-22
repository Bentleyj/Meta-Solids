#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec3 normal;
varying vec3 position;

void main(){
	// get original vertex postion
	vec4 pos = gl_Vertex;

	pos = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;

	position = vec3(pos);

	normal = vec3(gl_NormalMatrix * gl_Normal);


	//gl_FrontColor = (percentColor) * col1 + (1.0 - percentColor) * col2 + vec4(brightnessModifier);//col1;

	gl_Position = pos;//gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
