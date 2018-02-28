#version 120

uniform sampler2DRect inputTexture;

void main() {
    
    vec2 uv = gl_FragCoord.xy;
    vec2 uvAbs = uv;
        
    //float dx = 20;
    //float dy = 20;
    //vec2 coord = vec2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));
    vec3 tc = texture2DRect(inputTexture, uvAbs).rgb;
    vec3 tc1 = texture2DRect(inputTexture, vec2(uvAbs.x + 1, uvAbs.y)).rgb;
    vec3 tc2 = texture2DRect(inputTexture, vec2(uvAbs.x - 1, uvAbs.y)).rgb;
    vec3 tc3 = texture2DRect(inputTexture, vec2(uvAbs.x, uvAbs.y + 1)).rgb;
    vec3 tc4 = texture2DRect(inputTexture, vec2(uvAbs.x, uvAbs.y - 1)).rgb;
    
    // tc = tc;//(tc + tc1 + tc2 + tc3 + tc4) / 5.0;

    gl_FragColor = vec4(tc, 1.0);
}
