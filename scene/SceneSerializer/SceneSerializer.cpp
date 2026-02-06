#include "sceneSerializer.hpp"

bool SceneSerializer::saveScene(Scene& scene, const std::filesystem::path& path, Format format){
    switch(format){
        case Format::JSON:
            return saveJson(scene, path);
        case Format::BINARY:
            return false;
        default:
            return false;
    }
}

bool SceneSerializer::saveJson(Scene& scene, const std::filesystem::path& path){
    std::cout << "> Trying to save to JSON file\n";
    try{
        nlohmann::json j;

        // Convert scene to JSON
        j["version"] = 1;
        j["entities"] = serializeEntities(scene);
        j["programs"] = serializePrograms();
        j["meshes"] = serializeMeshes();

        std::ofstream file(path);
        file << j.dump(2);

        //? For release:
        // std::ofstream compactFile(path.string() + ".compact");
        // compactFile << j.dump(-1);

        return true;
    }catch(const std::exception& e){
        std::cerr << "Failed to save scene: " << e.what() << std::endl;
        return false;
    }
}

bool SceneSerializer::loadScene(Scene& scene, const std::filesystem::path& path){
    std::cout << "> Trying to load from JSON file\n";
    try{
        std::ifstream file(path);
        if(!file.is_open()){
            throw std::runtime_error("Failed to open scene file!");
            return false;
        }

        nlohmann::json j;
        file >> j;
        deserializeEntities(scene, j);

        return true;
    }catch(const std::exception& e){
        throw std::runtime_error(std::string("Failed to load scene: ") + e.what());
        return false;
    }
}