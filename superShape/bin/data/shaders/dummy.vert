#version 120
#extension GL_EXT_gpu_shader4 : enable

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;//gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

}