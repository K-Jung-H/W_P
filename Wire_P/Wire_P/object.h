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
struct Vertex 
{
    float x, y, z;
};

struct Normal
{
    float nx, ny, nz;
};

struct TexCoord {
    float u, v;
};

struct VertexData 
{
    Vertex vertex;
    Normal normal;
    TexCoord texCoord;
};

struct Face
{
    int vertexIndex[3];
    int normalIndex[3];
    int texCoordIndex[3];
};

struct Cube
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<VertexData> vertexData;
};

bool LoadOBJ(const std::string& filename, Cube* c);
void Check_Data(const Cube C);

extern Cube cube;