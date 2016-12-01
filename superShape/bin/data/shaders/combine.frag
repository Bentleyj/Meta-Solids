#version 120

uniform sampler2DRect u_Object;
uniform sampler2DRect u_Rim;
uniform vec2 u_Res;

void main()
{
	vec2 pos = gl_FragCoord.xy;

	pos.y = u_Res.y - pos.y;

	vec4 colObject = texture2DRect(u_Object, pos);
	vec4 colRim = texture2DRect(u_Rim, pos);

	vec4 combinedColor = colObject + colRim;

	gl_FragColor = combinedColor;
}