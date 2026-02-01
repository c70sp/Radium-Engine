#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

#include "sceneStruts.hpp"

class Scene{
    public:
        Entity* createEntity(const std::string& name);
        void removeEntity(Entity& ent);

        Entity* getEntity(const std::string& name);

        void saveToFile(const std::string& path);
        void loadFromFile(const std::string& path);

        
        // ffs, I hate C++, need to put this shit into the header file for the template to work.. hhhh        
        void addComponent(Entity& ent, const auto& component){
            using T = std::decay_t<decltype(component)>;
            
            if constexpr (std::is_same_v<T, TransformComponent>) transforms[ent.id] = component;
            else if constexpr (std::is_same_v<T, MeshComponent>) meshes[ent.id] = component;
            else if constexpr (std::is_same_v<T, MaterialComponent>) materials[ent.id] = component;
            else std::cerr << "Tried to add unsupported component!\n";
        }
        template<typename T>
        T& getComponent(Entity& ent){
            if constexpr (std::is_same_v<T, TransformComponent>) return transforms[ent.id];
            else if constexpr (std::is_same_v<T, MeshComponent>) return meshes[ent.id];
            else if constexpr (std::is_same_v<T, MaterialComponent>) return materials[ent.id];
            else std::cerr << "Component not found by getComponent()!\n";
        }
        void removeComponent();
        
        std::unordered_map<uint32_t, TransformComponent> transforms;
        std::unordered_map<uint32_t, MeshComponent> meshes;
        std::unordered_map<uint32_t, MaterialComponent> materials;

        std::vector<Entity> entities;
        uint32_t lastEntity = 0;
    private:
};