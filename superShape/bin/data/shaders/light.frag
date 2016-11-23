#version 120

varying vec3 position;
varying vec3 normal;

uniform vec3 l_position;

uniform vec3 m_ambient; //gl_FrontMaterial
uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform float shininess;

uniform vec3 l_ambient; //gl_Lightsource[]
uniform vec3 l_diffuse;
uniform vec3 l_specular;

uniform float brightnessModifier = 0.0;

void main() {

	float dist = length(position-l_position);
	float attenuation = 1.0 / 2.0;// (1.0 + 0.001*dist + 0.01*dist*dist);// ;
	vec3 ambient = m_ambient*l_ambient;

	vec3 lightPos = l_position;//(vec4(l_position, 1.0) * gl_ModelViewMatrix).xyz;
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
	vec3 reflection = reflect(-surf2light, norm); //**//
	float scont = pow(max(0.0, dot(surf2view, reflection)), shininess);

	vec3 specular = scont * l_specular * m_specular;

	vec4 colLit = vec4(ambient + diffuse + specular*attenuation, 1.0);
	//vec4 colFlat = vec4(1.0, 1.0, 1.0, 1.0);
	gl_FragColor = colLit;
}