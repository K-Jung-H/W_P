#pragma once
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable:4996)
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

extern GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
extern GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
extern GLuint shaderProgramID; //--- ���̴� ���α׷�

/*
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // ���� ����
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // ���İ� �ִ� ����
*/
