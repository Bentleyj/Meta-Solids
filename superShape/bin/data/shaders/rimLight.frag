#version 120

varying vec3 position;
varying vec3 normal;

uniform vec3 l_ambient;

void main() {
	vec3 norm = normalize(normal);

	vec3 surf2view = normalize(-position);

	float rcont = 1.0 - max(-dot(surf2view, norm), 0.0);

	rcont = smoothstep(0.8, 1.0, rcont);
	vec3 rim = vec3(rcont);//l_ambient * rcont / 2.0;

	gl_FragColor = vec4(rim, 1.0);//colLit;
}