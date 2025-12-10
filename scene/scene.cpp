#include "scene.hpp"

Entity* Scene::createEntity(SceneStruct& scene){
    Entity ent;
    scene.entities.emplace_back(ent);
    return &scene.entities.back();
}

void Scene::removeEntity(Entity& ent){

}