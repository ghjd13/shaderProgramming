#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawTriangle(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawParticles(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawFS(float x, float y, float z, float size, float r, float g, float b, float a);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_VBOTriangle = 0;
	GLuint m_TriangleShader = 0;

	GLuint m_VBOparticle = 0;

	//Fragment Shader
	GLuint m_VBOFS = 0;
	GLuint m_FSShader = 0;

	//RainDrop
	float m_DropPoints[1000 * 4];

	//Textures
	GLuint m_RgbTexture = 0;
	GLuint m_NumTexture[10];
	GLuint m_NumsTexture = 0;
};

