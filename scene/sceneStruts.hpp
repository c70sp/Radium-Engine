#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct MeshComponent{
    std::string meshName;
    uint32_t meshID;
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
    std::string name;
};

struct SceneInfo{
    std::string name;
};

struct EntityDesc{
    std::optional<TransformComponent> transform;
    std::optional<MeshComponent> mesh;
    std::optional<MaterialComponent> material;
    std::string name;
};