#include "scene.hpp"





Entity* Scene::createEntity(const std::string& name){
    Entity ent;

    ent.name = name;
    ent.id = lastEntity;
    lastEntity++;

    entities.emplace_back(ent);
    return &entities.back();
}

Entity* Scene::getEntity(const std::string& name){
    for(auto& e : entities){
        if(e.name == name){
            return &e;
        }
    }

    return nullptr;
}