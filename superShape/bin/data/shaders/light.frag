#version 120

varying vec3 position;
varying vec3 normal;

uniform vec3 l_position;
uniform vec3 moon_position;
uniform vec3 c_position;

uniform vec3 m_ambient;
uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform float shininess;

uniform vec3 l_ambient;
uniform vec3 l_diffuse;
uniform vec3 l_specular;

uniform vec3 moon_ambient;
uniform vec3 moon_diffuse;
uniform vec3 moon_specular;

vec4 getColor(vec3 pos, vec3 _ambient, vec3 _diffuse, vec3 _specular) {
    float dist = length(position-pos);
    float attenuation = 1.0 / 8.0f;
    vec3 ambient = m_ambient*_ambient;
    
    vec3 lightPos = pos;
    vec3 surf2light;
    
    surf2light = normalize(position - lightPos);
    
    vec3 norm = normalize(normal);
    float dcont = max(0.0, dot(norm, surf2light));
    vec3 diffuse = dcont * m_diffuse * _diffuse;
    
    vec3 surf2view = normalize(-position);
    vec3 reflection = reflect(-surf2light, norm);
    float scont = pow(1.0 - max(0.0, dot(surf2view, reflection)), shininess);
    
    vec3 specular = scont * m_specular * _specular;
    
    float rcont = 1.0 - max(-dot(surf2view, norm), 0.0);
    
    vec3 rim = vec3(smoothstep(0.8, 1.0, rcont));
    
    vec4 colLit = vec4(ambient + diffuse + specular*attenuation, 1.0);
    
    return colLit;
}


void main() {
    
    vec4 colL = getColor(l_position, l_ambient, l_diffuse, l_specular);
    vec4 colM = getColor(moon_position, moon_ambient, moon_diffuse, moon_specular);
    
    vec4 colLit = (colL + colM);

	gl_FragColor = colLit;
}
