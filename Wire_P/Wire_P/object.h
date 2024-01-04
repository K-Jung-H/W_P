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

struct Object
{
    int N; //vertex num
    GLuint VAO;
    GLuint VBO;
    std::vector<Face> faces;
    std::vector<VertexData> vertexData;

    glm::vec3 pos = { 0.0, 0.0, 0.0 };
    float X_angle = 0;
    float Y_angle = 0;
    float Z_angle = 0;


};

bool LoadOBJ(const std::string& filename, Object* c);
void Check_Data(const Object C);
void BuildOBJ(const std::string& filename, Object* c);
void DrawOBJ(const Object c, unsigned int Model_Transform, int objColorLocation, glm::mat4 transfrom);
