#pragma once

// C++ STL
#include <vector>
#include <string>
#include <memory>

// 3rd party libs
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "./../../scene/sceneStruts.hpp"

struct Texture{
    GLuint id;
    int width;
    int height;
    int channels;
    std::string type; // diffuse, specular, normal, height, etc.
    std::string path;
};

enum class IndexType{
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    UNSIGNED_INT
};

struct Mesh3D{
    uint32_t id;

    GLuint VAO, VBO, EBO, norm;
    int indexSize;

    GLuint program;
    glm::mat4 transforms = glm::mat4(1.0f);
    IndexType indexType = IndexType::UNSIGNED_INT;

    bool hasTexture = false;
    std::shared_ptr<Texture> texture;

    std::string name;
};

struct AssimpMesh{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    std::string name;
};








struct cpuMesh{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
};

struct gpuMesh{
    GLuint VAO, VBO, EBO;
    uint32_t indexSize;
};

struct meshInfo{
    uint32_t cpuMeshID;
    std::optional<uint32_t> gpuMeshID;
    std::string displayName; // from OBJ objects
    std::string internalName; // display + cpuMeshID
};

/**
 * Forward decl for renderable.
 */
struct TransformComponent;
struct MeshComponentNew;
struct MaterialComponent;

/**
 * The dataset that gets send to the renderer from either the game directly or the editor.
 */
struct Renderable{
    TransformComponent* trans;
    MeshComponentNew* mesh;
    MaterialComponent* mat;
};