#version 120

uniform vec2 u_Resolution;
uniform sampler2DRect u_TextureMask;
uniform sampler2DRect u_TextureBackground;

void main() {
	//vec2 p = (-u_Resolution.xy + 2.0*gl_FragCoord.xy) / u_Resolution.y;
	vec2 p = gl_FragCoord.xy / u_Resolution;
	vec4 col1 = texture2DRect(u_TextureMask, vec2((p.x) * u_Resolution.x, (1.0 - p.y) * u_Resolution.y));
	vec4 col2 = texture2DRect(u_TextureBackground, p * u_Resolution);

	gl_FragColor = vec4(col1.x, col2.yz, 1.);
}