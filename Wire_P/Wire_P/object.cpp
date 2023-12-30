#include "object.h"
#include <iomanip>
Cube cube;

bool LoadOBJ(const std::string& filename, Cube* c) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::vector<Vertex> vertices; // 임시로 저장할 벡터
    std::vector<Normal> normals;  // 임시로 저장할 벡터
    std::vector<TexCoord> texCoords;  // 임시로 저장할 벡터
    std::vector<Face> faces; // 임시로 저장할 벡터
    std::vector<GLuint> indices; //EBO에 연결할 임시 벡터


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") {
            Vertex vertex;
            stream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex); // 임시로 저장
        }
        else if (type == "vn") {
            Normal normal;
            stream >> normal.nx >> normal.ny >> normal.nz;
            normals.push_back(normal); // 임시로 저장
        }
        else if (type == "vt") {
            TexCoord texCoord;
            stream >> texCoord.u >> texCoord.v;
            texCoords.push_back(texCoord); // 임시로 저장
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
            faces.push_back(face); // 임시로 저장
        }
    }
    file.close();

    // VBO에 저장될 데이터 초기화
    std::vector<VertexData> vertexData;

    // 각 정보를 하나의 데이터로 합치기
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



    // 각 face의 정점 인덱스를 EBO에 추가
    for (const Face& face : faces) {
        for (int i = 0; i < 3; ++i) {
            indices.push_back(face.vertexIndex[i] - 1);
        }
    }

    // EBO 생성 및 데이터 전송
    glGenBuffers(1, &c->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);



    //// VBO에 모든 데이터 저장
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