#version 330

layout(location=0) out vec4 FragColor;

uniform float u_Time;
uniform sampler2D u_RGBTex;
uniform sampler2D u_CurrNumTex;
uniform sampler2D u_NumsTex;
uniform int u_InputNum;

in vec2 v_TPos;
in vec2 v_Tex;

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

void TextureSampling()
{
	vec4 c0;
	vec4 c1;
	vec4 c2;
	vec4 c3;
	vec4 c4;

	float offsetX = 0.01;

	c0 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 2.0, v_Tex.y));
    c1 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 1.0, v_Tex.y));
    c2 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 0.0, v_Tex.y));
    c3 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 1.0, v_Tex.y));
    c4 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 3.0, v_Tex.y));

	vec4 sum = c0 + c1 + c2 + c3 + c4;
    sum = sum / 5.0;

	FragColor = sum;
}
void TextureQ1()
{
	float tx = 1.0 - v_Tex.x;
	float ty = 1.0 - abs(v_Tex.y - 0.5) * 2.0;;

	vec2 newTex = vec2(tx, ty);

	FragColor = texture(u_RGBTex, newTex);
}

void TextureQ2()
{

	float tx = 1 - fract(v_Tex.x*3);
	float ty = 1.0 - v_Tex.y/3;

	float offSetX = 0;
    float offSetY = (2 - floor(v_TPos.x * 3))/3;

	vec2 newTex = vec2(tx + offSetX , ty + offSetY);
    FragColor = texture(u_RGBTex, newTex);
}

void TextureQ3()
{
    float tx = 1 - fract(v_TPos.x * 3);
    float ty = v_TPos.y/3;

    float offSetX = 0;
    float offSetY = floor((v_TPos.x * 3))/3;

    vec2 newTex = vec2(tx + offSetX , ty + offSetY);
    FragColor = texture(u_RGBTex, newTex);
}
void TextureQ4()
{
	float resolX = 2;
	float resolY = 5;
	float shear = 0.5 * u_Time;

    float offSetX = fract(ceil(v_Tex.y*resolY)*shear);
    float offSetY = 0;

    float tx = 1 - fract(v_TPos.x * resolX + offSetX);
    float ty = fract(v_TPos.y * resolY + offSetY);


    vec2 newTex = vec2(tx , ty);
    FragColor = texture(u_RGBTex, newTex);
}

void Num()
{
    float tx = 1 - v_Tex.x;
    float ty = v_Tex.y;

    float offSetX = 0;
    float offSetY = 0;

    vec2 newTex = vec2(tx + offSetX , ty + offSetY);
    FragColor = texture(u_CurrNumTex, newTex);
}

void Nums()
{
	float index = float(u_InputNum);
	float tx = 1 - v_Tex.x / 5;
    float ty = v_Tex.y / 2;

    float offSetX = fract(index / 5.0);
    float offSetY = floor(index / 5.0) / 2.0;

    vec2 newTex = vec2(tx + offSetX , ty + offSetY);
    FragColor = texture(u_NumsTex, newTex);
}

void main()
{
	Nums();
}
