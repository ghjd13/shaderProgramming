#version 330

layout(location=0) out vec4 FragColor;

uniform float u_Time;

in vec2 v_TPos;

float C_PI = 3.141592;

uniform vec4 u_DropInfo[1000]; //vec4(x, y, sT, lT)

void simple(){
	if(v_TPos.x + v_TPos.y > 1){
		FragColor = vec4(0);
	}
	else{
		FragColor = vec4(v_TPos, 0, 1);
	}
}

void Pattern(){
	float lineCountH = 10;
	float lineCountV = 1;
	float lineWidth = 0.1;
	lineCountH = lineCountH /2;
	lineCountV = lineCountV /2;
	lineWidth = 50 / lineWidth;
	float per = -0.5 * C_PI;

	//FragColor = vec4(v_TPos, 0, 1);
	float grey = pow(
		abs(sin((v_TPos.y*C_PI*2+per)
			*lineCountH))
				,lineWidth);
	float grey1 = pow(
		abs(sin((v_TPos.x*C_PI*2+per)
			*lineCountV))
				,lineWidth);
	float resColor = max(grey, grey1);
	FragColor = vec4(resColor);
}

void Circle(){
	vec2 center = vec2(0.5, 0.5);
	vec2 currPos = v_TPos.xy;
	float d = distance(center,currPos);
	float linewidth = 0.01;
	float radius = 0.1;

	if(d>(radius - linewidth) && d < radius){
		FragColor = vec4(1);
	}
	else{
		FragColor = vec4(0);
	}
}

void CircleSin(){
	vec2 center = vec2(0.5, 0.5);
	vec2 currPos = v_TPos.xy;
	float d = distance(center,currPos);
	float speed = 10;

	float value = abs(sin(d * 2 * C_PI * 10
					- u_Time * speed));

	FragColor = vec4(pow(value, 256));
}

void RainDrop(){
	float accum = 0;
	// RainDrop
	for(int i = 0; i < 1000; i++){
		float lTime = u_DropInfo[i].w;
		float sTime = u_DropInfo[i].z;
		float newTime = u_Time - sTime;

		if(newTime > 0){
			newTime = fract(newTime/lTime);

			float oneMinus = 1 - newTime;
			float t = newTime * lTime;

			vec2 center = u_DropInfo[i].xy;
			vec2 currPos = v_TPos.xy;

			float range = t/0.5;
			float d = distance(center,currPos);

			float fade = 10 * clamp(range - d, 0, 1);

			float value = abs(sin(d * 2 * C_PI * 50 -  t * 500));

			accum += value * fade * oneMinus;
		} else{
		}
	}
	FragColor = vec4(accum);
}

void main()
{
	RainDrop();
}
