#include "sceneSerializer.hpp"


/**
 * Returns a string of the serialized entities.
 * Well that is a little over compilcated I guess, could be done a lot simpler.
 * But for sake of consistancy with the other functions I will keep it like this :D
 */
nlohmann::json SceneSerializer::serializeEntities(Scene& scene){
    nlohmann::json j = nlohmann::json::array();

    /**
     * TODO: Make scene.entities private. Should not be public.
     * TODO: Same goes for the maps holding components.
     */
    if(scene.entitiesNew.size() <= 0){
        throw std::runtime_error("0 entities to save!");
    }

    for(auto e : scene.entitiesNew){
        j.push_back(serializeEntity(scene, e));
    }

    return j;
}

nlohmann::json SceneSerializer::serializeEntity(Scene& scene, uint32_t entID){
    nlohmann::json j;

    j["id"] = entID;

    j["transformComponent"] = serializeTransform(scene.getComponent<TransformComponent>(entID));
    j["meshComponent"] = serializeMeshInfo(scene.getComponent<MeshComponentNew>(entID));
    j["materialComponent"] = serializeMaterialInfo(scene.getComponent<MaterialComponent>(entID));

    // scene.getComponent<TransformComponent>(e)

    return j;
}


/**
 * Returns a string of the serialized transforms
 */
nlohmann::json SceneSerializer::serializeTransform(TransformComponent& tc){
    nlohmann::json j;

    j["position"] = {tc.pos.x, tc.pos.y, tc.pos.z};
    j["rotation"] = {tc.rot.x, tc.rot.y, tc.rot.z};
    j["scale"] = {tc.scale.x, tc.scale.y, tc.scale.z};

    return j;
}

/**
 * Returns a string of the serialized meshes
 */
nlohmann::json SceneSerializer::serializeMeshInfo(MeshComponentNew& mc){
    nlohmann::json j;

    const auto& meshInfo = ENGINE->getMeshManager().getMeshInfo(mc.meshInfoID);
    j["meshInfoID"] = mc.meshInfoID;
    j["meshPathInfo"] = meshInfo.path;
    j["meshInternalNameInfo"] = meshInfo.internalName;

    return j;
}


/**
 * Returns a string of the serialized materials
 */
nlohmann::json SceneSerializer::serializeMaterialInfo(MaterialComponent& mc){
    nlohmann::json j;

    j["programName"] = mc.programName;
    j["programID"] = mc.programID;

    return j;
}




nlohmann::json SceneSerializer::serializePrograms(){
    nlohmann::json j;

    auto& sm = ENGINE->getShaderManager();
    for(auto& prog : *sm.getAllPrograms()){
        j.push_back(serializeProgram(prog));
    }

    return j;
}

nlohmann::json SceneSerializer::serializeProgram(ProgramInfo prog){
    nlohmann::json j;
    
    j["progName"] = prog.name;
    
    nlohmann::json k;
    for(auto& shader : prog.programShaders){
        k.push_back(shader.name);
    }
    j["progShaders"] = k;


    return j;
}

nlohmann::json SceneSerializer::serializeMeshes(){
    nlohmann::json j;

    // TODO: not sure how smart this is... could be pretty slow... Though it only needs to run on save, so no big deal (for now)
    auto& mm = ENGINE->getMeshManager();
    std::vector<meshInfo*> meshInfoArr = mm.getAllMeshInfo();
    for(auto& mesh : meshInfoArr){
        j.push_back(serializeMesh(*mesh));
    }

    return j;
}

nlohmann::json SceneSerializer::serializeMesh(meshInfo& mi){
    nlohmann::json j;

    j["meshPath"] = mi.path;
    j["meshInternalName"] = mi.internalName;
    j["meshDisplayName"] = mi.displayName;

    return j;
}