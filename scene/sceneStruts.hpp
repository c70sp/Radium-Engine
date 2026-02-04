#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "./../engine/meshManager/mesh3d.hpp"

struct MeshComponent{
    std::string meshName;
    uint32_t meshID;
};

struct MeshComponentNew{
    uint32_t meshInfoID;
};

struct TransformComponent{
    glm::vec3 pos = glm::vec3(0);
    glm::vec3 rot = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
};

struct MaterialComponent{
    std::string programName;
    uint32_t programID;
};

struct Entity{
    uint32_t id;
    // std::string name;
};

struct SceneInfo{
    std::string name;
};

/**
 * This is kind of just a "transfer protocol".
 * This is used so I only need to create a single event and all of the basic info is already there.
 */
struct EntityDesc{
    TransformComponent transform;
    MeshComponentNew mesh;
    MaterialComponent material;
};