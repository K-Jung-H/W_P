#include "object.h"
#include "stb_image.h"
#include "shader.h"


bool LoadOBJ(const std::string& filename, Object* c) {
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
    for (const Face& face : faces) 
    {
        for (int i = 0; i < 3; ++i) 
        {
            VertexData vertexDataItem;
            vertexDataItem.vertex = vertices[face.vertexIndex[i] - 1];

            if (!normals.empty()) 
            {
                vertexDataItem.normal = normals[face.normalIndex[i] - 1];
            }
            if (!texCoords.empty()) 
            {
                vertexDataItem.texCoord = texCoords[face.texCoordIndex[i] - 1];
            }
            vertexData.push_back(vertexDataItem);
        }
    }
    c->vertexData = vertexData;
    c->faces = faces;
    c->N = vertexData.size();
    std::cout << c->N << std::endl;
    return true;
}

void BuildOBJ(const std::string& filename, Object* c)
{
    if (LoadOBJ(filename, c))
    {
        // VAO ����
        glGenVertexArrays(1, &c->VAO);
        glBindVertexArray(c->VAO);

        glGenBuffers(1, &c->VBO);


        glBindBuffer(GL_ARRAY_BUFFER, c->VBO); //�ϳ��� ���ۿ� ��� ������ ����
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * c->vertexData.size(), c->vertexData.data(), GL_STATIC_DRAW);

        // ���ؽ� �Ӽ� ���� -> ��� ���� �Ǿ��ִ���
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, vertex));
        glEnableVertexAttribArray(0);

        // ��� �Ӽ� ���� -> ��� ���� �Ǿ��ִ���
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
        glEnableVertexAttribArray(1);

        // �ؽ�ó ��ǥ �Ӽ� -> ��� ���� �Ǿ��ִ���
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoord));
        glEnableVertexAttribArray(2);

        // VAO ����ε�
        //glBindVertexArray(0);

        //=========================================================================

        int widthImage, heightImage, numberOfChannel;
        glGenTextures(3, c->texture);

        glBindTexture(GL_TEXTURE_2D, c->texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load("resource/Body.png", &widthImage, &heightImage, &numberOfChannel, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        int tLocation = glGetUniformLocation(shaderProgramID, "texture1");
        glUniform1i(tLocation, 0);
        stbi_image_free(data);


        glBindTexture(GL_TEXTURE_2D, c->texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("resource/Eye.png", &widthImage, &heightImage, &numberOfChannel, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        tLocation = glGetUniformLocation(shaderProgramID, "texture1");
        glUniform1i(tLocation, 0);
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, c->texture[2]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("resource/Mouth.png", &widthImage, &heightImage, &numberOfChannel, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        tLocation = glGetUniformLocation(shaderProgramID, "texture1");
        glUniform1i(tLocation, 0);
        stbi_image_free(data);

    }
    else
    {
        std::cout << "fail: " << filename << std::endl;
        exit(1);
    }
}

void Check_Data(const Object C)
{
    std::cout << "Vertex Data:" << std::endl;
    for (size_t i = 0; i < C.vertexData.size(); ++i)
    {
        const VertexData& vertexData = C.vertexData[i];
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

void DrawOBJ(const Object c, unsigned int Model_Transform, int objColorLocation, glm::mat4 transfrom)
{
    
    transfrom = glm::rotate(transfrom, glm::radians(c.X_angle), glm::vec3(1.0, 0.0, 0.0));
    transfrom = glm::rotate(transfrom, glm::radians(c.Y_angle), glm::vec3(0.0, 1.0, 0.0));
    transfrom = glm::rotate(transfrom, glm::radians(c.Z_angle), glm::vec3(0.0, 0.0, 1.0));
    transfrom = glm::scale(transfrom, glm::vec3(0.1, 0.1, 0.1));
    glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
    glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);

    //8694; //8538

    glBindVertexArray(c.VAO);
    glDrawArrays(GL_TRIANGLES, c.temp, 12792 - c.temp);
    std::cout << c.temp << std::endl;
    
    glBindTexture(GL_TEXTURE_2D, c.texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, 8538);

    glBindTexture(GL_TEXTURE_2D, c.texture[1]); // ��
    glDrawArrays(GL_TRIANGLES, 8538, 54);

    glBindTexture(GL_TEXTURE_2D, c.texture[2]); // ��
    glDrawArrays(GL_TRIANGLES, 8592, 48);

    glBindTexture(GL_TEXTURE_2D, c.texture[1]); // ��
    glDrawArrays(GL_TRIANGLES, 8640, 54);

    glBindTexture(GL_TEXTURE_2D, c.texture[0]);
    glDrawArrays(GL_TRIANGLES, 8694, c.vertexData.size() - 8694);




    //glDrawArrays(GL_TRIANGLES, 0, c.vertexData.size());
    glBindVertexArray(0);
}



