// C++ STL
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "meshManager.hpp"
#include "./../core/common.hpp"

void MeshManager::loadModel(const std::string& filepath){
    meshLoader.loadModel(filepath);
    std::vector<AssimpMesh> tempMeshes = meshLoader.extractMeshes();

    // extractMeshes() only returns meshes from last loadModel call, that is why I push it into the cache
    for(auto& tempMesh : tempMeshes){
        cachedMeshes.push_back(tempMesh);
    }
}

Mesh3D MeshManager::getModel(const std::string& name){
    for(auto& mesh : cachedMeshes){
        if(mesh.name == name){
            return createMesh(mesh);
        }
    }
    std::cerr << "Mesh with name: " << name << " not found" << std::endl;

    Mesh3D mesh;
    return mesh;
}

std::vector<Mesh3D> MeshManager::getAllModels(){
    std::vector<Mesh3D> meshes;

    for(auto& mesh : cachedMeshes){
        meshes.push_back(createMesh(mesh));
    }

    return meshes;
}

AssimpMesh MeshManager::getModelData(const std::string& name){
    for(auto& mesh : cachedMeshes){
        if(mesh.name == name){
            return mesh;
        }
    }
    std::cerr << "Mesh with name: " << name << " not found" << std::endl;
    AssimpMesh mesh;
    return mesh;
}

std::vector<AssimpMesh> MeshManager::getAllModelData(){
    return cachedMeshes;
}

Mesh3D MeshManager::createMesh(AssimpMesh& mesh){
    Mesh3D returnMesh;

    std::vector<GLuint> indices = mesh.indices;
    std::vector<GLfloat> vertices;
    for(unsigned int i = 0; i < mesh.vertices.size() / 3; i++){
        vertices.push_back(mesh.vertices[i*3+0]);
        vertices.push_back(mesh.vertices[i*3+1]);
        vertices.push_back(mesh.vertices[i*3+2]);

        vertices.push_back(mesh.normals[i*3+0]);
        vertices.push_back(mesh.normals[i*3+1]);
        vertices.push_back(mesh.normals[i*3+2]);

        vertices.push_back(mesh.texCoords[i*2+0]);
        vertices.push_back(mesh.texCoords[i*2+1]);
    }
    glGenVertexArrays(1, &returnMesh.VAO);
    glBindVertexArray(returnMesh.VAO);

    glGenBuffers(1, &returnMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, returnMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // vvv Verticies vvv
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)0);
    // vvv Normals vvv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    // vvv UV Coordinates vvv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    glGenBuffers(1, &returnMesh.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


    glBindVertexArray(0);

    returnMesh.indexSize = indices.size();
    returnMesh.name = mesh.name;

    returnMesh.id = lastMesh;
    meshStorage[lastMesh] = returnMesh;
    lastMesh++;

    return returnMesh;
}

// // Mesh3D MeshManager::loadModel(const std::string& location, GLuint program){
// //     std::vector<GLfloat> vertexData;
// //     std::vector<GLuint> indexData;
// //     retrieveMesh3D(location, vertexData, indexData);
    
// //     std::vector<GLfloat> norm;
// //     return createMesh(vertexData, indexData, norm, program);
// // };

// Mesh3D MeshManager::createModel(const std::vector<GLfloat> vertices, const std::vector<GLuint> indices, const std::vector<GLfloat> norm, GLuint prog){
//     return createMesh(vertices, indices, norm, prog);
// }

// Mesh3D MeshManager::createMesh(const std::vector<GLfloat> vertices, const std::vector<GLuint> indices, const std::vector<GLfloat> norm, GLuint prog){
//     Mesh3D mesh;

//     glGenVertexArrays(1, &mesh.VAO);
//     glBindVertexArray(mesh.VAO);

//     glGenBuffers(1, &mesh.VBO);
//     glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
    
//     glGenBuffers(1, &mesh.norm);
//     glBindBuffer(GL_ARRAY_BUFFER, mesh.norm);
//     glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(GLfloat), norm.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

//     glGenBuffers(1, &mesh.EBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//     glBindVertexArray(0);

//     mesh.indexSize = indices.size();
//     mesh.program = prog;

//     return mesh;
// }

// void MeshManager::retrieveMesh3D(const std::string& location, std::vector<GLfloat>& vertexDataArr, std::vector<GLuint>& indexBufferArr){
//     std::string objectData = loadFileAsString(location);

//     std::vector<glm::vec3> vertices;
//     std::vector<glm::vec2> texCoords;
//     std::vector<glm::vec3> normals;

//     std::istringstream stream(objectData);
//     std::string line;

//     while (std::getline(stream, line)) {
//         std::istringstream lineStream(line);
//         std::string prefix;
//         lineStream >> prefix;

//         if (prefix == "v") {
//             GLfloat x, y, z;
//             lineStream >> x >> y >> z;
//             vertices.emplace_back(x, y, z);
//         } else if (prefix == "vt") {
//             GLfloat u, v;
//             lineStream >> u >> v;
//             texCoords.emplace_back(u, v);
//         } else if (prefix == "vn") {
//             GLfloat nx, ny, nz;
//             lineStream >> nx >> ny >> nz;
//             normals.emplace_back(nx, ny, nz);
//         } else if (prefix == "f") {
//             GLuint vIndex[3], vtIndex[3], vnIndex[3];

//             for (int i = 0; i < 3; ++i) {
//                 std::string vertexStr;
//                 lineStream >> vertexStr;

//                 if (vertexStr.empty()) break;

//                 // Parse the vertex/texcoord/normal index (v/vt/vn)
//                 sscanf(vertexStr.c_str(), "%u/%u/%u", &vIndex[i], &vtIndex[i], &vnIndex[i]);

//                 // OBJ indices are 1-based, OpenGL uses 0-based indices
//                 --vIndex[i];
//                 --vtIndex[i];
//                 --vnIndex[i];

//                 // Append interleaved vertex data to the vertexDataArr
//                 const glm::vec3& vertex = vertices[vIndex[i]];
//                 const glm::vec2& texCoord = texCoords[vtIndex[i]];
                
//                 vertexDataArr.push_back(vertex.x);
//                 vertexDataArr.push_back(vertex.y);
//                 vertexDataArr.push_back(vertex.z);

//                 // vertexDataArr.push_back(1.0f);
//                 // vertexDataArr.push_back(1.0f);
//                 // vertexDataArr.push_back(1.0f);

//                 // vertexDataArr.push_back(texCoord.x);
//                 // vertexDataArr.push_back(texCoord.y);
//             }

//             // Add indices for this face
//             indexBufferArr.push_back(indexBufferArr.size());
//             indexBufferArr.push_back(indexBufferArr.size());
//             indexBufferArr.push_back(indexBufferArr.size());
//         }
//     }
// }