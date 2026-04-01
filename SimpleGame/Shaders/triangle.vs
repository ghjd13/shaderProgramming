#version 330

in vec3 a_Position;
uniform float u_Time;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV1;
in float a_RV2;

out float v_Grey;
out vec2 vUV;

const float c_PI = 3.141592;
const float c_G = 9.8;


float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void Sin1()
{
	float startTime = random(vec2(a_RV, a_Mass))*a_RV2;
	float newTime = u_Time - startTime;

	if(newTime > 0){
		float t = mod(newTime * 10.0, 2.0);
		float peroid = (a_RV1 * 2.0) + 1.0;
		float amp = t*0.1*(a_RV - 0.5)*2;

		vec4 newPosition;

		newPosition.x = a_Position.x + t;

		newPosition.y = a_Position.y + amp * sin(peroid * t * 2.0 * c_PI);

		newPosition.z = 0;
		newPosition.w = 1;

		gl_Position = newPosition;
	}else{
		gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
	}
}

void rotate()
{
    float startTime = random(vec2(a_RV, a_Mass)) * a_RV2;
    float newTime = u_Time - startTime;

    if(newTime > 0.0){
        float moveSpeed = 5.0;
        float t = mod(newTime * moveSpeed, 2.0); 
        
        float travelTime = t / moveSpeed;
        float launchTime = u_Time - travelTime;

        // [수정 포인트 1] 기본 회전 각도
        float rotAngle = launchTime * 15.0; 

        // [수정 포인트 2] 반대 방향 로직 추가
        // a_RV1은 0~1 사이 랜덤값이므로, 0.5를 기준으로 두 방향으로 나눕니다.
        if (a_RV1 < 0.5) {
            rotAngle += c_PI; // 180도 반대 방향
        }

        float period = (a_RV1 * 2.0) + 1.0;
        
        // [참고] 현재 amp는 멀어질수록 넓어지는 방식입니다. 
        // 좁아지게 하려면 (1.0 - t/2.0)을 곱하세요.
        float amp = t * 0.3 * (a_RV - 0.5) * 2.0;
        
        float localX = t;
        float localY = amp * sin(period * t * 2.0 * c_PI);

        vec4 newPosition;
        newPosition.x = localX * cos(rotAngle) - localY * sin(rotAngle);
        newPosition.y = localX * sin(rotAngle) + localY * cos(rotAngle);
        
        newPosition.z = 0.0;
        newPosition.w = 1.0;

        gl_Position = newPosition;

		v_Grey = 1-t;

		float scale = random(vec2(a_Mass, a_RV)) * 2.0 + 0.5;
		gl_PointSize = scale;
    } else {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
		v_Grey = 0;
    }
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
	float startTime = random(vec2(a_RV, a_Mass))*a_RV2;
	float newTime = u_Time - startTime;

	if(newTime > 0){

		float lifeTime = a_RV2 * 4.0 + 0.5;
		float t = fract(newTime/lifeTime); //0 ~ lifeTime 구간
		float tt = t*t;

		float scale;

		float vx, vy;
		float sx, sy;

		scale = random(vec2(a_Mass, a_RV)) * 2.0 + 0.5;

		vx = a_Vel.x/3;
		vy = a_Vel.y/3;

		sx = a_Position.x + sin(a_RV * 2* c_PI);
		sy = a_Position.y + cos(a_RV * 2 * c_PI);

		newPos.x = sx + a_Position.x + vx * t;
		newPos.y = sy + a_Position.y + vy * t - (0.5 * c_G * tt);
		newPos.z = 0;
		newPos.w = 1;

		gl_Position = newPos;

		gl_PointSize = scale;
	}
	else{
		gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
	}
}

void test(){
	float startTime = random(vec2(a_RV, a_Mass))*a_RV2;
	float newTime = u_Time - startTime;

	if(newTime > 0){
		// 애니메이션 주기 (3초)
		float lifeTime = 3.0;
		// 0.0 ~ 1.0 사이의 정규화된 시간
		float t = fract(newTime / lifeTime); 

		vec4 newPos;
		
		// 1. 위치 계산: x축 방향으로 -1.0에서 1.0까지 이동
		float startX = -1.0 + (a_RV - 0.5) * 0.5; // 약간의 랜덤 시작 위치
		newPos.x = startX + (t * 2.0); 
		// y축은 Mass에 따라 약간 위아래로 물결침
		newPos.y = a_Position.y + sin(t * c_PI * 4.0 + a_Mass) * 0.1;
		newPos.z = 0;
		newPos.w = 1;

		gl_Position = newPos;

		// 2. 크기 계산
		float scale = random(vec2(a_Mass, a_RV)) * 2.0 + 1.0;
		gl_PointSize = scale;

		// 3. [가장 중요] vUV 계산!
		// Fragment Shader의 step(0.5, vUV.x) 로직을 활용하기 위해
		// 현재 파티클의 x 위치(-1.0 ~ 1.0 범위)를 UV 범위(0.0 ~ 1.0)로 변환합니다.
		// 공식: (값 - 최소값) / (최대값 - 최소값) -> (newPos.x - (-1.0)) / (1.0 - (-1.0))
		vUV.x = (newPos.x + 1.0) / 2.0; 
		
		// y값은 현재 이펙트에서 사용하지 않으므로 0.5로 고정
		vUV.y = 0.5; 

		// v_Grey값도 시간 변화에 따라 설정해 줍니다.
		v_Grey = 1.0 - t;

	}
	else{
		// 시작 전에는 화면 밖으로 치웁니다.
		gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
		// [중요] 모든 분기에서 vUV에 값을 써주어야 오류가 나지 않습니다.
		vUV = vec2(0.0);
		v_Grey = 0.0;
	}
}

void main()
{
	test();
}
