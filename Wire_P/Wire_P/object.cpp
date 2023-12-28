#include "object.h"

Cube cube;

bool LoadOBJ(const std::string& filename, Cube* c) 
{
    std::ifstream file(filename);


    if (!file.is_open()) 
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") 
        {
            Vertex vertex;
            stream >> vertex.x >> vertex.y >> vertex.z;
            c->vertices.push_back(vertex);
        }
        else if (type == "vn") 
        {
            Normal normal;
            stream >> normal.nx >> normal.ny >> normal.nz;
            c->normals.push_back(normal);
        }
        else if (type == "vt") 
        {
            TexCoord texCoord;
            stream >> texCoord.u >> texCoord.v;
            c->texCoords.push_back(texCoord);
        }
        else if (type == "f") 
        {
            Face face;
            for (int i = 0; i < 3; ++i) 
            {
                stream >> face.vertexIndex[i];
                if (stream.peek() == '/') 
                {
                    stream.ignore(); // Skip '/'
                    if (stream.peek() != '/')
                        stream >> face.texCoordIndex[i];
                    if (stream.peek() == '/') 
                    {
                        stream.ignore(); // Skip '/'
                        stream >> face.normalIndex[i];
                    }
                }
            }
            c->faces.push_back(face);
        }
    }
    file.close();

    std::cout << "Cube data: "<< std::endl;
    std::cout << "v: " << c->vertices.size() << std::endl;
    std::cout << "n: " << c->normals.size() << std::endl;
    std::cout << "vt: " << c->texCoords.size() << std::endl;
    return true;
}
