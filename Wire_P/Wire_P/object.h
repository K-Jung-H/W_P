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
#include "player.h"


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

class Object
{


public:
    int N; //vertex num
    GLuint VAO;
    GLuint VBO;
    std::vector<Face> faces;
    std::vector<VertexData> vertexData;

    glm::vec3 pos = { 0.0, 0.0, 0.0 };
    float X_angle = 0;
    float Y_angle = 180;
    float Z_angle = 0; 

    virtual bool LoadOBJ(const std::string& filename);
    void Build(const std::string& filename);

    virtual void LoadTexture();
    virtual void Draw(unsigned int Model_Transform, int objColorLocation, Player player);
    virtual void Draw(unsigned int Model_Transform, int objColorLocation, Camera player);

};

class Emolga : public Object
{
public:
    unsigned int texture[5];

    void LoadTexture() override;
    void Draw(unsigned int Model_Transform, int objColorLocation, Player player)  override;

};

class Background : public Object
{
public:
    unsigned int texture[2];
    void LoadTexture() override;
    void Draw(unsigned int Model_Transform, int objColorLocation, Player player)  override;
};