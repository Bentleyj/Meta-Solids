#version 120

varying vec3 position;
varying vec3 normal;

void main() {
	vec3 norm = normalize(normal);

	vec3 surf2view = normalize(-position);

	float rcont = 1.0 - max(-dot(surf2view, norm), 0.0);

	vec3 rim = vec3(smoothstep(0.8, 1.0, rcont));

	gl_FragColor = vec4(rim, 1.0);//colLit;
}