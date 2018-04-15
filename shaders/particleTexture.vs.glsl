#version 330
// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

in vec2 in_world_pos;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 projection;

void main()
{
	texcoord = in_texcoord;

	float scale = 0.005;

	vec3 pos = projection  *  vec3((in_position.xy * scale) + (in_world_pos.xy  ), 1.0);


	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}