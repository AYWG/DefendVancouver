#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 transform;
uniform mat3 projection;
uniform int frameCount;
uniform float frameWidth;
uniform float frameHeight;

void main()
{

    int FRAMEX = frameCount % 3;
    int FRAMEY = frameCount / 3;
	texcoord = in_texcoord;
	texcoord.x += FRAMEX * frameWidth;
	texcoord.y += FRAMEY * frameHeight;
	vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}