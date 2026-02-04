#define GLM_ENABLE_EXPERIMENTAL
#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include <glm/gtx/string_cast.hpp>

#include "sceneStruts.hpp"

#include "./../engine/engine.hpp"

/**
 * This is the scene object itself.
 * The functions here are for:
 *  - creating entities
 *  - deleting entities
 *  - adding and removing components to entities
 *  - saving the scene to a file
 *  - loading a scene file
 * 
 * This is NOT for:
 *  - Managing all the scenes
 *  - Creating scenes
 *  - Deleting scenes
 */
class Scene{
    public:
        Scene();

        void createEventListeners();
        void createEntityNew(const SpawnEntityEvent& e);

        Entity* createEntity(const std::string& name);
        void removeEntity(Entity& ent);

        Entity* getEntity(const std::string& name);

        void saveToFile(const std::string& path);
        void loadFromFile(const std::string& path);

        
        // ffs, I hate C++, need to put this shit into the header file for the template to work.. hhhh        
        void addComponent(uint32_t id, const auto& component){
            using T = std::decay_t<decltype(component)>;
            
            if constexpr (std::is_same_v<T, TransformComponent>) transforms[id] = component;
            else if constexpr (std::is_same_v<T, MeshComponent>) meshes[id] = component;
            else if constexpr (std::is_same_v<T, MeshComponentNew>) meshesNew[id] = component;
            else if constexpr (std::is_same_v<T, MaterialComponent>) materials[id] = component;
            else std::cerr << "Tried to add unsupported component!\n";
        }
        template<typename T>
        T& getComponent(uint32_t id){
            if constexpr (std::is_same_v<T, TransformComponent>) return transforms[id];
            else if constexpr (std::is_same_v<T, MeshComponent>) return meshes[id];
            else if constexpr (std::is_same_v<T, MeshComponentNew>) return meshesNew[id];
            else if constexpr (std::is_same_v<T, MaterialComponent>) return materials[id];
            else std::cerr << "Component not found by getComponent()!\n";
        }
        void removeComponent();
        
        /**
         * Potentially switch to vector of structs?
         * Would allow fast lookup (since renderer loops over these)
         * struct transformContainer{
         *      uint32_t id;
         *      TransformComponent;
         * };
         */
        std::unordered_map<uint32_t, TransformComponent> transforms;
        std::unordered_map<uint32_t, MeshComponent> meshes;
        std::unordered_map<uint32_t, MeshComponentNew> meshesNew;
        std::unordered_map<uint32_t, MaterialComponent> materials;

        std::vector<Entity> entities;
        std::vector<uint32_t> entitiesNew;
        uint32_t lastEntity = 0;







        void createEntity(uint32_t ID);
    private:
};