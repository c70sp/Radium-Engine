#include "./editorActions.hpp"

void EditorActions::spawnEntity(){
    auto& mm = ENGINE->getMeshManager();

    std::vector<uint32_t> id;
    std::vector<uint32_t> tempID;
    for(int i = 0; i < 1; i++){
        // tempID = mm.parseObj("C:/dev/projects/C++/OpenGL/Radium Engine/.assets/objects/SpaceShip1 - Export.obj");
        tempID = mm.parseObj("C:/dev/projects/C++/OpenGL/Radium Engine/.assets/objects/cube.obj");
        id.insert(id.end(), tempID.begin(), tempID.end());
    }

    EntityDesc desc;

    desc.transform = TransformComponent{};
    desc.transform.scale = glm::vec3(0.1);
    desc.transform.pos.x = randNum() * 5;
    desc.transform.pos.y = randNum() * 5;
    desc.transform.pos.z = randNum() * 5;

    desc.material = MaterialComponent{};
    GLuint basicProgram = ENGINE->getShaderManager().getProgram("basic", "v_vert.glsl", "f_frag.glsl");
    desc.material.programID = basicProgram;

    /**
     * compileMesh can take both ID and name.
     * If ID- simply search the unordered_map for the fitting ID
     * If Name- iterate over map and search for a name. Names must be unique.
     * Later: make seperate unordered_map<std::string (internal name), uint32_t ID> nameToID for quick retrieval.
     *      => will be O(1) and not O(n).
     * compileMesh will take the fitting CPU mesh and turn it into a GPU mesh.
     * GPU mesh contains:
     *      - VAO
     *      - VBO
     *      - EBO/IBO
     *      - index size
     * They get stored as an unordered_map<uint32_t (ID), gpuMesh> gpuMeshes;
     */
    for(auto& mesh : id){
        desc.mesh.meshInfoID = mm.compileMesh(mesh); // or compileMesh(name);
        ENGINE->getEventSystem().emit<SpawnEntityEvent>(desc);
    }    
}

float EditorActions::randNum(){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 100); // define the range

    int intNum = distr(gen);
    float returnNum = (float)intNum / 100;

    return returnNum;
}