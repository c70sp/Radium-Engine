#pragma once

#include <iostream>

#include "sceneStruts.hpp"

class Scene{
    public:
        Entity* createEntity(SceneStruct& scene);
        void removeEntity(Entity& ent);

        // For now only one scene, so no need to specifiy the scene ig
        void saveToFile(const std::string& path);
        void loadFromFile(const std::string& path);





        // Usage: scene.addComponent<TransformComponent>(entity, ...construction args...);
        template<typename Component, typename... Args>
        void addComponent(Entity& ent, Args&&... args){
            auto& storage = getStorage<Component>();
            storage[ent.id] = Component(std::forward<Args>(args)...);
        }

        // Usage: scene.removeComponent<TransformComponent>(entity);
        template<typename Component>
        void removeComponent(Entity& ent){
            std::cout << "Removed comp" << std::endl;
        }

        // Usage:: scene.getComponent<TransformComponent>(entity)
        template<typename Component>
        Component* getComponent(Entity& ent){
            auto& storage = getStorage<Component>();
            auto it = storage.find(ent.id);
            if(it != storage.end()) return &it->second;
            return nullptr;
        }





        // vvvvvvvvvvvvvvv ATTRIBS vvvvvvvvvvvvvvv

    private:
        // vvvvv getStorage() vvvvv
        template<typename Component>
        std::unordered_map<uint32_t, Component>& getStorage() {
            // unique static map per component type
            static std::unordered_map<uint32_t, Component> storage;
            return storage;
        }
};

template<>
std::unordered_map<uint32_t, MeshComponent>& SceneStruct::getStorage<MeshComponent>() {
    return meshes;
}
