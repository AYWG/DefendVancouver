#version 330
uniform sampler2D sampler0;
uniform vec2 texOffset;
varying vec4 vertColor;
varying vec4 vertTexCoord;

uniform int blursize;
uniform int horizontalPass;
uniform float sigma;

const float pi = 3.14159265;
void main()
{
    float blurpixelsperside = float(blursize/2);
    vec2 blurmultiplyvec = 0 < horizontalPass ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    
    vec3 gaussiancalc;
    gaussiancalc.x = 1.0 / (sqrt(2.0 * pi) * sigma);
    gaussiancalc.y = exp(-0.5 / (sigma * sigma));
    gaussiancalc.z = gaussiancalc.y * gaussiancalc.y;
    
    vec4 avgvalue = vec4 (0.0, 0.0, 0.0, 0.0);
    float coefficientSum = 0.0;
    
    // central sample first
    avgvalue += texture2D(sampler0, vertTexCoord.st) * gaussiancalc.x;
    coefficientSum += gaussiancalc.x;
    gaussiancalc.xy *= gaussiancalc.yz;
    
    for (float i = 1.0; i <= blurpixelsperside; i++) {
        avgvalue += texture2D(sampler0, vertTexCoord.st - i * texOffset * blurmultiplyvec) * gaussiancalc.x;
        avgvalue += texture2D(sampler0, vertTexCoord.st + i * texOffset * blurmultiplyvec) * gaussiancalc.x;
        coefficientSum += 2.0 * gaussiancalc.x;
        gaussiancalc.xy *= gaussiancalc.yz;
    }
    gl_FragColor = avgvalue / coefficientSum;
}
