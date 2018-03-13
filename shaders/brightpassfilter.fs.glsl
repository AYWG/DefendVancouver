#version 330
// From vertex shader
in vec2 texcoord;
varying vec4 vertColor;
varying vec4 vertTexCoord;

uniform float brightPassThreshold;
// Application data

varying vec3 fcolor;


// Output color
layout(location = 0) out  vec4 color;

void main()
{
    vec3 luminancevec = vec3(0.2125, 0.7154, 0.0721);
    vec4 c = texture2D(sampler0, vertTexCoord.st) * vertColor;
    
    float luminance = dot(luminancevec, c.xyz);
    luminance = max(0.0, luminance - brightPassThreshold);
    c.xyz *= sign(luminance);
    color = c;
//    color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
}
