#version 330

layout(location=0) out vec4 FragColor;

in float v_Grey;
in vec2 vUV;

uniform vec4 u_Color;

void main()
{
	float split = step(0.5, vUV.x);
	vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), split);
	FragColor = vec4(color, 1.0);
}
