#version 120
#extension GL_EXT_gpu_shader4 : enable

void main() {
	vec4 pos = gl_Vertex;
	pos = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;

	gl_FrontColor = gl_Color;
	gl_Position = pos;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}