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
    std::cout << ">>> Recieved create event!\n";

    // First, create the entity ID
    uint32_t ent = lastEntity++;
    
    // Store it in the vector
    entitiesNew.emplace_back(ent);
    
    // Add components
    addComponent(ent, e.entityDesc.transform);
    addComponent(ent, e.entityDesc.mesh);
    addComponent(ent, e.entityDesc.material);

    std::cout << entitiesNew.back() << std::endl;
}

Entity* Scene::createEntity(const std::string& name){
    Entity ent;

    // ent.name = name;
    // ent.id = lastEntity;
    // lastEntity++;

    entities.emplace_back(ent);
    return &entities.back();
}

Entity* Scene::getEntity(const std::string& name){
    // for(auto& e : entities){
    //     if(e.name == name){
    //         return &e;
    //     }
    // }

    return nullptr;
}