#include "sceneSerializer.hpp"

void SceneSerializer::deserializeEntities(Scene& scene, const nlohmann::json& j){
    try{
        scene.transforms.clear();
        scene.meshesNew.clear();
        scene.materials.clear();
        scene.entitiesNew.clear();
    
        if(!j.contains("version") || j["version"].get<int>() != 1){
            throw std::runtime_error("Warning: Scene version mismatch or missing version!\n");
        }

        deserializePrograms(j);
        deserializeMeshes(j);

        // if(j.contains("meshes") && j["meshes"].is_array()){
            // for(const auto& meshEntry : j["meshes"]){
                // std::cout << meshEntry["meshInternalName"] << std::endl;
            // }
        // }
    
        if(j.contains("entities") && j["entities"].is_array()){
            for(const auto& eJson : j["entities"]){
                if(!eJson.contains("id") || !eJson["id"].is_number_integer()){
                    throw std::runtime_error("Warning: Entity missing ID, skipping!\n");
                    continue;
                }
    
                uint32_t id = eJson["id"].get<uint32_t>();
                scene.entitiesNew.push_back(id);
    
                if(id >= scene.lastEntity) scene.lastEntity = id + 1;
    
                deserializeEntity(scene, id, eJson);

            }
            std::cout << ">>>>> Amount of Entity transform components: " << scene.transforms.size() << std::endl;
            std::cout << ">>>>> Amount of Entity mesh components: " << scene.meshesNew.size() << std::endl;
            std::cout << ">>>>> Amount of Entity material components: " << scene.materials.size() << std::endl;
        }
    }catch(const nlohmann::json::exception& e){
        throw std::runtime_error(std::string("JSON parsing error: ") + e.what());
    }catch(const std::exception& e){
        throw std::runtime_error(std::string("Failed to deserialize entities: ") + e.what());
    };
}

void SceneSerializer::deserializeEntity(Scene& scene, uint32_t ID, const nlohmann::json& j){
    if(j.contains("transformComponent")) deserializeTransform(scene, ID, j["transformComponent"]);
    if(j.contains("meshComponent")) deserializeMeshInfo(scene, ID, j["meshComponent"]);
    if(j.contains("materialComponent")) deserializeMaterialInfo(scene, ID, j["materialComponent"]);
}

void SceneSerializer::deserializeTransform(Scene& scene, uint32_t ID, const nlohmann::json& j){
    TransformComponent tc;

    tc.pos = glm::vec3(j["position"][0], j["position"][1], j["position"][2]);
    tc.rot = glm::vec3(j["rotation"][0], j["rotation"][1], j["rotation"][2]);
    tc.scale = glm::vec3(j["scale"][0], j["scale"][1], j["scale"][2]);

    scene.addComponent(ID, tc);
}

void SceneSerializer::deserializeMeshInfo(Scene& scene, uint32_t ID, const nlohmann::json& j){
    MeshComponentNew mc;

    mc.meshInfoID = j["meshInfoID"];
    
    scene.addComponent(ID, mc);
}

void SceneSerializer::deserializeMaterialInfo(Scene& scene, uint32_t ID, const nlohmann::json& j){
    MaterialComponent mc;

    mc.programName = j["programName"];

    mc.programID = ENGINE->getShaderManager().getProgramIDFromName(mc.programName);
    
    scene.addComponent(ID, mc);
}

void SceneSerializer::deserializePrograms(const nlohmann::json& j){
    if(j.contains("programs") && j["programs"].is_array()){
        ENGINE->getShaderManager().compileAllShaders();
        
        // vvvvv Find program object vvvvv
        for(const auto& pJson : j["programs"]){
    
            std::string progName = pJson["progName"];
    
            // vvvvv Get all shaders of program vvvvv
            std::vector<std::string> shaderList;
            for(const auto& sJson : pJson["progShaders"]){
                if constexpr(staticConfig::debugging)
                    std::cout << "Shader of program " << progName << ": " << sJson.get<std::string>() << std::endl;
                    shaderList.push_back(sJson.get<std::string>());
            }
            ENGINE->getShaderManager().getProgramFromArray(progName, shaderList);
        }
    }
}

void SceneSerializer::deserializeMeshes(const nlohmann::json& j){
    if(j.contains("meshes") && j["meshes"].is_array()){
        for(const auto& mesh : j["meshes"]){
            std::cout << mesh["meshInternalName"] << std::endl;

            uint32_t id = ENGINE->getMeshManager().parseObj(mesh["meshPath"].get<std::string>())[0];
            ENGINE->getMeshManager().compileMesh(id);
        }
    }
}