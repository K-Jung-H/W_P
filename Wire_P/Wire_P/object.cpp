#include "object.h"
#include "stb_image.h"
#include "shader.h"




bool Object::LoadOBJ(const std::string& filename) 
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::vector<Vertex> t_vertices; // 임시로 저장할 벡터
    std::vector<Normal> t_normals;  // 임시로 저장할 벡터
    std::vector<TexCoord> t_texCoords;  // 임시로 저장할 벡터
    std::vector<Face> t_faces; // 임시로 저장할 벡터
    std::vector<GLuint> t_indices; //EBO에 연결할 임시 벡터


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") {
            Vertex vertex;
            stream >> vertex.x >> vertex.y >> vertex.z;
            t_vertices.push_back(vertex); // 임시로 저장
        }
        else if (type == "vn") {
            Normal normal;
            stream >> normal.nx >> normal.ny >> normal.nz;
            t_normals.push_back(normal); // 임시로 저장
        }
        else if (type == "vt") {
            TexCoord texCoord;
            stream >> texCoord.u >> texCoord.v;
            t_texCoords.push_back(texCoord); // 임시로 저장
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
            t_faces.push_back(face); // 임시로 저장
        }
    }
    file.close();

    // VBO에 저장될 데이터 초기화
    std::vector<VertexData> t_vertexData;

    // 각 정보를 하나의 데이터로 합치기
    for (const Face& face : t_faces) 
    {
        for (int i = 0; i < 3; ++i) 
        {
            VertexData vertexDataItem;
            vertexDataItem.vertex = t_vertices[face.vertexIndex[i] - 1];

            if (!t_normals.empty())
            {
                vertexDataItem.normal = t_normals[face.normalIndex[i] - 1];
            }
            if (!t_texCoords.empty())
            {
                vertexDataItem.texCoord = t_texCoords[face.texCoordIndex[i] - 1];
            }
            t_vertexData.push_back(vertexDataItem);
        }
    }
    vertexData = t_vertexData;
    faces = t_faces;
    N = t_vertexData.size();
    std::cout << N << std::endl;
    return true;
}

void Object::Build(const std::string& filename)
{
    if (LoadOBJ(filename))
    {
        // VAO 설정
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);


        glBindBuffer(GL_ARRAY_BUFFER, VBO); //하나의 버퍼에 모든 데이터 저장
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

        // 버텍스 속성 설정 -> 어떻게 저장 되어있는지
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, vertex));
        glEnableVertexAttribArray(0);

        // 노멀 속성 설정 -> 어떻게 저장 되어있는지
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
        glEnableVertexAttribArray(1);

        // 텍스처 좌표 속성 -> 어떻게 저장 되어있는지
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoord));
        glEnableVertexAttribArray(2);

        // VAO 언바인드
        //glBindVertexArray(0);

        //=========================================================================

       /* int widthImage, heightImage, numberOfChannel;
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
        stbi_image_free(data);*/

    }
    else
    {
        std::cout << "fail: " << filename << std::endl;
        exit(1);
    }
}

void Object::LoadTexture()
{
    std::cout << "Load Texture" << std::endl;
    return;
}

void Object::Draw(unsigned int Model_Transform, int objColorLocation, Player player)
{
    int Texture_mode_Location = glGetUniformLocation(shaderProgramID, "Use_Texture_Mode"); 
    glUniform1i(Texture_mode_Location, 1);//텍스쳐 사용 X

    glm::mat4 transfrom = glm::mat4(1.0f);

    //transfrom = glm::rotate(transfrom, glm::radians(X_angle), glm::vec3(1.0, 0.0, 0.0));
    //transfrom = glm::rotate(transfrom, glm::radians(Y_angle), glm::vec3(0.0, 1.0, 0.0));
    //transfrom = glm::rotate(transfrom, glm::radians(Z_angle), glm::vec3(0.0, 0.0, 1.0));
    //transfrom = glm::translate(transfrom, player.Get_dir() + player.Get_pos());
    transfrom = glm::scale(transfrom, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
    glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);


    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, N);

    return;
}

void Object::Draw(unsigned int Model_Transform, int objColorLocation, Camera cam)
{
    int Texture_mode_Location = glGetUniformLocation(shaderProgramID, "Use_Texture_Mode");
    glUniform1i(Texture_mode_Location, 1);//텍스쳐 사용 X

    glm::mat4 transfrom = glm::mat4(1.0f);

    //transfrom = glm::rotate(transfrom, glm::radians(X_angle), glm::vec3(1.0, 0.0, 0.0));
    //transfrom = glm::rotate(transfrom, glm::radians(Y_angle), glm::vec3(0.0, 1.0, 0.0));
    //transfrom = glm::rotate(transfrom, glm::radians(Z_angle), glm::vec3(0.0, 0.0, 1.0));
    transfrom = glm::translate(transfrom, glm::vec3(cam.Pos));
    transfrom = glm::scale(transfrom, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
    glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);


    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, N);

    return;
}


void Emolga::LoadTexture()
{
    int widthImage, heightImage, numberOfChannel;
    glGenTextures(3, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
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


    glBindTexture(GL_TEXTURE_2D, texture[1]);
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

    glBindTexture(GL_TEXTURE_2D, texture[2]);
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
    return;
}

void Emolga::Draw(unsigned int Model_Transform, int objColorLocation, Player player)
{
    int Texture_mode_Location = glGetUniformLocation(shaderProgramID, "Use_Texture_Mode"); 
    glUniform1i(Texture_mode_Location, 1);//텍스쳐 사용 O
    
    glm::mat4 transfrom = glm::mat4(1.0f);

    transfrom = glm::translate(transfrom, glm::vec3(player.Get_pos()));
    //transfrom = glm::rotate(transfrom, glm::radians(X_angle), glm::vec3(1.0, 0.0, 0.0));
    transfrom = glm::rotate(transfrom, glm::radians(player.Get_angle()), glm::vec3(0.0, 1.0, 0.0));
    //transfrom = glm::rotate(transfrom, glm::radians(Z_angle), glm::vec3(0.0, 0.0, 1.0));
    transfrom = glm::scale(transfrom, glm::vec3(0.05, 0.05, 0.05));
    glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
    glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);

   // std::cout << "Draw_Emolga" << std::endl;
    glBindVertexArray(VAO);
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, 8538);

    glBindTexture(GL_TEXTURE_2D, texture[1]); // 눈
    glDrawArrays(GL_TRIANGLES, 8538, 54);

    glBindTexture(GL_TEXTURE_2D, texture[2]); // 입
    glDrawArrays(GL_TRIANGLES, 8592, 48);

    glBindTexture(GL_TEXTURE_2D, texture[1]); // 눈
    glDrawArrays(GL_TRIANGLES, 8640, 54);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 8694, vertexData.size() - 8694);

    glBindVertexArray(0);
}



void Background::LoadTexture()
{
    int widthImage, heightImage, numberOfChannel;
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resource/background.png", &widthImage, &heightImage, &numberOfChannel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    int tLocation = glGetUniformLocation(shaderProgramID, "texture1");
    glUniform1i(tLocation, 0);
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D, texture[1]);
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

    return;
}

void Background::Draw(unsigned int Model_Transform, int objColorLocation, Player player)
{
    int Texture_mode_Location = glGetUniformLocation(shaderProgramID, "Use_Texture_Mode");
    glUniform1i(Texture_mode_Location, 1);//텍스쳐 사용 O

    glm::mat4 transfrom = glm::mat4(1.0f);

    transfrom = glm::scale(transfrom, glm::vec3(10.0f, 10.0f, 10.0f));
    glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
    glUniform3f(objColorLocation, 1.0f, 1.0f, 1.0f);

    std::cout << "Draw_background" << std::endl;
    glBindVertexArray(VAO);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 6, 6);


    glBindVertexArray(0);
}

