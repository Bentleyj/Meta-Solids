#version 120

varying vec3 position;
varying vec3 normal;

uniform vec3 l_position;

uniform vec3 m_ambient;
uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform float shininess;

uniform vec3 l_ambient;
uniform vec3 l_diffuse;
uniform vec3 l_specular;

void main() {

	float dist = length(position-l_position);
	float attenuation = 1.0 / 6.0f;//(1.0 + 0.001*dist + 0.01*dist*dist);// ;
	vec3 ambient = m_ambient*l_ambient;

	vec3 lightPos = l_position;
	vec3 surf2light;
	if (gl_FrontFacing) {
		surf2light = normalize(position - lightPos);
	}
	else {
		surf2light = normalize(lightPos - position);
	}
	vec3 norm = normalize(normal);
	float dcont = max(0.0, dot(norm, surf2light));
	vec3 diffuse = dcont * m_diffuse * l_diffuse;

	vec3 surf2view = normalize(-position);
	vec3 reflection = reflect(-surf2light, norm);
	float scont = pow(1.0 - max(0.0, dot(surf2view, reflection)), shininess);

	vec3 specular = scont * l_specular * m_specular;

	float rcont = 1.0 - max(-dot(surf2view, norm), 0.0);

	vec3 rim = vec3(smoothstep(0.8, 1.0, rcont));

	vec4 colLit = vec4(ambient + diffuse + specular*attenuation, 1.0);

	gl_FragColor = colLit;
}