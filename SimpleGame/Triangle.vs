#version 330

in vec3 a_Position;
uniform vec4 u_Trans;

void main()
{
	vec4 newPosition;
	newPosition.xy = vec4(a_position, 1);
	gl_Position = newPosition;
}
