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
#include <iomanip>
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
    int N;
    GLuint VAO;
    GLuint VBO;
    std::vector<Face> faces;
    std::vector<VertexData> vertexData;
};

bool LoadOBJ(const std::string& filename, Cube* c);
void Check_Data(const Cube C);
void BuildOBJ(const std::string& filename, Cube* c);
