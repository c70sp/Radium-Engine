#define GLM_ENABLE_EXPERIMENTAL
#pragma once

// C++ STL
#include <iostream>
#include <string>
#include <filesystem>

// 3rd party libs
#include "./../.libs/json.hpp"
#include <glm/gtx/string_cast.hpp>

// Own libs
#include "./../scene.hpp"

class Scene;

/**
 * This will for now be JSON.
 * Can later be Binary Blob with JSON Metadata.
 * Or go all in with pure Binary.
 * https://chat.deepseek.com/a/chat/s/bef1d848-ad7e-4c1e-83cf-74fd4185a3b3
 * ^ short chat about the different ways to properly store the data
 */

class SceneSerializer{
    public:
        enum class Format{
            JSON,
            BINARY
        };

        bool saveScene(Scene& scene, const std::filesystem::path& path, Format format = Format::JSON);
        bool loadScene(Scene& scene, const std::filesystem::path& path);
    private:
        bool saveJson(Scene& scene, const std::filesystem::path& path);




        // vvvvvvvvvv SERIALIZER vvvvvvvvvv
        nlohmann::json serializeEntities(Scene& scene);
        nlohmann::json serializeEntity(Scene& scene, uint32_t entID);

        nlohmann::json serializeTransform(TransformComponent& tc);              // Transform
        nlohmann::json serializeMeshInfo(MeshComponentNew& mc);                 // Mesh
        nlohmann::json serializeMaterialInfo(MaterialComponent& mc);            // Material

        nlohmann::json serializePrograms();
        nlohmann::json serializeProgram(ProgramInfo prog);

        nlohmann::json serializeMeshes();
        nlohmann::json serializeMesh(meshInfo& mi);
        // ^^^^^^^^^^ SERIALIZER ^^^^^^^^^^

        // vvvvvvvvvv DESERIALIZER vvvvvvvvvv
        void deserializeEntities(Scene& scene, const nlohmann::json& j);
        void deserializeEntity(Scene& scene, uint32_t entID, const nlohmann::json& j);

        void deserializeTransform(Scene& scene, uint32_t ID, const nlohmann::json& j);          // Transform
        void deserializeMeshInfo(Scene& scene, uint32_t ID, const nlohmann::json& j);           // Mesh
        void deserializeMaterialInfo(Scene& scene, uint32_t ID, const nlohmann::json& j);       // Material 

        void deserializePrograms(const nlohmann::json& j);

        void deserializeMeshes(const nlohmann::json& j);
        // ^^^^^^^^^^ DESERIALIZER ^^^^^^^^^^
};