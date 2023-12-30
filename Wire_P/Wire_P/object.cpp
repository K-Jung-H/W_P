#include "object.h"
#include <iomanip>
Cube cube;

bool LoadOBJ(const std::string& filename, Cube* c) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::vector<Vertex> vertices; // �ӽ÷� ������ ����
    std::vector<Normal> normals;  // �ӽ÷� ������ ����
    std::vector<TexCoord> texCoords;  // �ӽ÷� ������ ����
    std::vector<Face> faces; // �ӽ÷� ������ ����
    std::vector<GLuint> indices; //EBO�� ������ �ӽ� ����


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") {
            Vertex vertex;
            stream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex); // �ӽ÷� ����
        }
        else if (type == "vn") {
            Normal normal;
            stream >> normal.nx >> normal.ny >> normal.nz;
            normals.push_back(normal); // �ӽ÷� ����
        }
        else if (type == "vt") {
            TexCoord texCoord;
            stream >> texCoord.u >> texCoord.v;
            texCoords.push_back(texCoord); // �ӽ÷� ����
        }
        else if (type == "f") {
            Face face;
            for (int i = 0; i < 3; ++i) {
                stream >> face.vertexIndex[i];
                if (stream.peek() == '/') {
                    stream.ignore(); // Skip '/'
                    if (stream.peek() != '/')
                        stream >> face.texCoordIndex[i];
                    if (stream.peek() == '/') {
                        stream.ignore(); // Skip '/'
                        stream >> face.normalIndex[i];
                    }
                }
            }
            faces.push_back(face); // �ӽ÷� ����
        }
    }
    file.close();

    // VBO�� ����� ������ �ʱ�ȭ
    std::vector<VertexData> vertexData;

    // �� ������ �ϳ��� �����ͷ� ��ġ��
    for (const Face& face : faces) {
        for (int i = 0; i < 3; ++i) {
            VertexData vertexDataItem;
            vertexDataItem.vertex = vertices[face.vertexIndex[i] - 1];
            if (!normals.empty()) {
                vertexDataItem.normal = normals[face.normalIndex[i] - 1];
            }
            if (!texCoords.empty()) {
                vertexDataItem.texCoord = texCoords[face.texCoordIndex[i] - 1];
            }
            vertexData.push_back(vertexDataItem);
        }
    }
    c->vertexData = vertexData;



    // �� face�� ���� �ε����� EBO�� �߰�
    for (const Face& face : faces) {
        for (int i = 0; i < 3; ++i) {
            indices.push_back(face.vertexIndex[i] - 1);
        }
    }

    // EBO ���� �� ������ ����
    glGenBuffers(1, &c->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);



    //// VBO�� ��� ������ ����
    //glBindBuffer(GL_ARRAY_BUFFER, c->VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), vertexData.data(), GL_STATIC_DRAW);

    return true;
}



void Check_Data(const Cube C)
{
    std::cout << "Vertex Data:" << std::endl;
    for (size_t i = 0; i < cube.vertexData.size(); ++i)
    {
        const VertexData& vertexData = cube.vertexData[i];
        std::cout <<'[' << i << "]: "
            << "x: " << std::setw(9) << vertexData.vertex.x
            << ", y: " << std::setw(9) << vertexData.vertex.y
            << ", z: " << std::setw(9) << vertexData.vertex.z
            << "  nx: " << std::setw(9) << vertexData.normal.nx
            << ", ny: " << std::setw(9) << vertexData.normal.ny
            << ", nz: " << std::setw(9) << vertexData.normal.nz
            << "  u: " << std::setw(9) << vertexData.texCoord.u
            << ", v: " << std::setw(9) << vertexData.texCoord.v
            << std::endl;
    }
    
}