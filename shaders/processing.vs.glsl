#version 330
// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 transform;
uniform mat3 projection;

attribute vec4 vertex;
attribute vec4 color;
varying vec4 vertColor;
varying vec4 vertTexCoord;

void main()
{
	texcoord = in_texcoord;
    vertColor = color;
    vertTexCoord = projection * vec4(texcoord, 1.0, 1.0);
	vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}
