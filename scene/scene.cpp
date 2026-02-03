#include "scene.hpp"

Scene::Scene(){
    createEventListeners();
}

void Scene::createEventListeners(){
    ENGINE->getEventSystem().subscribe<SpawnEntityEvent>([this](const SpawnEntityEvent& e){
        
        createEntityNew(e);
    });
}

void Scene::createEntityNew(const SpawnEntityEvent& e){
    std::cout << "Recieved entity create event!\n";
    // Entity* ent = createEntity(e.entityDesc.name);
}

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