#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Vertex {
	float x, y, z;
};

struct Normal {
	float nx, ny, nz;
};

struct TexCoord {
	float u, v;
};

struct Face {
	int vertexIndex[3];
	int normalIndex[3];
	int texCoordIndex[3];
};

struct Cube
{
	GLuint Vao;
	GLuint Vbo_p;
	GLuint Vbo_n;
	GLuint Vbo_vt;

	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<TexCoord> texCoords;
	std::vector<Face> faces;

};

bool LoadOBJ(const std::string& filename, Cube* c);


extern Cube cube;