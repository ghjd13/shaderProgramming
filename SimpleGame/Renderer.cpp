#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	//m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/Triangle.vs", "./Shaders/Triangle.fs");
	m_FSShader = CompileShaders("./Shaders/FS.vs", "./Shaders/FS.fs");
	
	//Gen Drop Info
	int index = 0;
	for (int i = 0; i < 1000; i++) {
		float x = (float)rand() / (float)RAND_MAX;
		float y = (float)rand() / (float)RAND_MAX;
		float sTime = 3 * (float)rand() / (float)RAND_MAX;
		float lTime = (float)rand() / (float)RAND_MAX;

		m_DropPoints[index] = x; index++;
		m_DropPoints[index] = y; index++;
		m_DropPoints[index] = sTime; index++;
		m_DropPoints[index] = lTime; index++;
	}

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_FSShader > 0)
	{
		m_Initialized = true;
	}

	
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	

	float centerX = 0;
	float centerY = 0;
	float size = 0.1;
	float mass = 1;

	float vx = 1;
	float vy = 3;

	float rv = 2;
	float rv1 = 2;
	float rv2 = 1;
	//데이터 준비
	float triangle[]
		=
	{
		centerX - size / 2, centerY - size / 2, 0, mass, vx, vy,
		centerX + size / 2, centerY - size / 2, 0, mass, vx, vy,
		centerX + size / 2, centerY + size / 2, 0, mass, vx, vy, //triangle 1

		centerX - size / 2, centerY - size / 2, 0, mass, vx, vy,
		centerX + size / 2, centerY + size / 2, 0, mass, vx, vy,
		centerX - size / 2, centerY + size / 2, 0, mass, vx, vy //triangle 2
	};

	glGenBuffers(1, &m_VBOTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	float rectFS[]
		=
	{
		-1.f, -1.f, 0.f,  1.f, 1.f, 
		 1.f,  1.f, 0.f,  0.f, 0.f, 
		-1.f,  1.f, 0.f,  1.f, 0.f, // Triangle 1

		-1.f, -1.f, 0.f,  1.f, 1.f,
		 1.f, -1.f, 0.f,  0.f, 1.f,
		 1.f,  1.f, 0.f,  0.f, 0.f  // Triangle 2
	};

	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectFS), rectFS, GL_STATIC_DRAW);


	// 3. 신규 파티클 데이터 생성 
	const int PARTICLE_COUNT = 1500;
	float* particleData = new float[PARTICLE_COUNT * 9];

	float radius = 0.5f; // 생성될 원의 반지름 (0.0 ~ 1.0 사이로 조절)
	float PI = 3.141592f;

	for (int i = 0; i < PARTICLE_COUNT; ++i)
	{
		float angle = ((float)rand() / RAND_MAX) * 2.0f * PI;

		particleData[i * 9 + 0] = 0; // x
		particleData[i * 9 + 1] = 0; // y
		particleData[i * 9 + 2] = 0.0f;                // z
		particleData[i * 9 + 3] = ((float)rand() / RAND_MAX) * 5.0f + 0.1f; // mass

		float speed = ((float)rand() / RAND_MAX) * 2.0f + 1.0f;
		particleData[i * 9 + 4] = cos(angle) * speed; // vx
		particleData[i * 9 + 5] = sin(angle) * speed; // vy

		particleData[i * 9 + 6] = ((float)rand() / RAND_MAX); // rv
		particleData[i * 9 + 7] = ((float)rand() / RAND_MAX); // rv1
		particleData[i * 9 + 8] = ((float)rand() / RAND_MAX); // rv2
	}
	glGenBuffers(1, &m_VBOparticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOparticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PARTICLE_COUNT * 8, particleData, GL_STATIC_DRAW);

	delete[] particleData; // 동적 할당 해제
	//데이터 준비 끝
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float g_time = 0;

void Renderer::DrawTriangle(
	float x, float y, float z, float size, 
	float r, float g, float b, float a)
{
	g_time += 0.0001; // 테스트용
	//Program select
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, g_time);

	int attribPosition = glGetAttribLocation(
		m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(
		m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(
		m_TriangleShader, "a_Vel");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);

	glVertexAttribPointer(attribPosition
		, 3, GL_FLOAT, GL_FALSE, 
		sizeof(float) * 6, 0);
	glVertexAttribPointer(attribMass
		, 1, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));
	glVertexAttribPointer(attribVel
		, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawParticles(
	float x, float y, float z, float size,
	float r, float g, float b, float a)
{
	g_time += 0.0001; // 셰이더 내부의 움직임을 위한 시간 갱신

	// 파티클도 기존 m_TriangleShader를 재사용합니다.
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, g_time);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");

	int attribRV = glGetAttribLocation(m_TriangleShader, "a_RV");
	int attribRV1 = glGetAttribLocation(m_TriangleShader, "a_RV1");
	int attribRV2 = glGetAttribLocation(m_TriangleShader, "a_RV2");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);

	glEnableVertexAttribArray(attribRV);
	glEnableVertexAttribArray(attribRV1);
	glEnableVertexAttribArray(attribRV2);

	// 위에서 만든 파티클 VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOparticle);

	// 구조는 기존과 완벽하게 동일합니다. (Stride: 6, Offset: 0, 3, 4)
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(attribRV, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(attribRV1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 7));
	glVertexAttribPointer(attribRV2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 8));

	glEnable(GL_PROGRAM_POINT_SIZE);
	// 점(Point) 형태로 5000개의 파티클을 한 번에 드로우 합니다.
	glDrawArrays(GL_POINTS, 0, 5000);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRV);
	glDisableVertexAttribArray(attribRV1);
	glDisableVertexAttribArray(attribRV2);
}

void Renderer::DrawFS(
	float x, float y, float z, float size,
	float r, float g, float b, float a)
{
	g_time += 0.0001; // 셰이더 내부의 움직임을 위한 시간 갱신

	// 파티클도 기존 m_TriangleShader를 재사용합니다.
	glUseProgram(m_FSShader);
	int uTime = glGetUniformLocation(m_FSShader, "u_Time");
	glUniform1f(uTime, g_time);
	int uPoints = glGetUniformLocation(m_FSShader, "u_DropInfo");
	glUniform4fv(uPoints, 1000, m_DropPoints);

	int attribPosition = glGetAttribLocation(m_FSShader, "a_Pos");
	int attribTPos = glGetAttribLocation(m_FSShader, "a_TPos");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTPos);

	// 위에서 만든 파티클 VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);

	// 구조는 기존과 완벽하게 동일합니다. (Stride: 6, Offset: 0, 3, 4)
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTPos);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}