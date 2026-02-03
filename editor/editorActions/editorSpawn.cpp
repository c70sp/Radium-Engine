#include "./editorActions.hpp"

void EditorActions::spawnEntity(){
    EntityDesc ent;
    ent.name = "jo";

    ent.mesh = MeshComponent{};
    ent.transform = TransformComponent{};
    ent.material = MaterialComponent{};
    
    ENGINE->getEventSystem().emit(SpawnEntityEvent{ent});
}