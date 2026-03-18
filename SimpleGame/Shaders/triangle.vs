#version 330

in vec3 a_Position;
uniform float u_Time;
in float a_Mass;
in vec2 a_Vel;


const float c_PI = 3.141592;
const float c_G = 9.8;

void Sin1()
{
	float t = u_Time * 2;

	vec4 newPosition;

	newPosition.x = a_Position.x + t;

	newPosition.y = a_Position.y + sin(t * 2 * 3.141592) * 0.5;

	newPosition.z = 0;
	newPosition.w = 1;

	gl_Position = newPosition;
}

void Sin2()
{
	float t = u_Time * 2;

	vec4 newPosition;

	newPosition.x = a_Position.x + t - 1;

	newPosition.y = a_Position.y + sin(t * 1 * 3.141592) * 0.5;

	newPosition.z = 0;
	newPosition.w = 1;

	gl_Position = newPosition;
}

void Sin3()
{
	float t = u_Time * 2;

	vec4 newPosition;

	float radius = 1.0;

	newPosition.x = a_Position.x + cos(t) * radius;
	newPosition.y = a_Position.y + sin(t) * radius;

	newPosition.z = 0.0;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void falling(){
	vec4 newPos;

	float t = mod(u_Time, 1.0);
	float tt = t*t;
	float vx, vy;

	vx = a_Vel.x;
    vy = a_Vel.y;

	newPos.x = a_Position.x + vx * t;
	newPos.y = a_Position.y + vy * t - (0.5 * c_G * tt);
	newPos.z = 0;
	newPos.w = 1;

	gl_Position = newPos;
}

void main()
{
	falling();
}
