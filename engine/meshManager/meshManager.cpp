#include "meshManager.hpp"

void MeshManager::loadModel(const std::string& filepath){
    meshLoader.loadModel(filepath);
    std::vector<AssimpMesh> tempMeshes = meshLoader.extractMeshes();

    // extractMeshes() only returns meshes from last loadModel call, that is why I push it into the cache
    for(auto& tempMesh : tempMeshes){
        cachedMeshes.push_back(tempMesh);
    }
}

Mesh3D MeshManager::getModel(const std::string& name){
    for(auto& mesh : cachedMeshes){
        if(mesh.name == name){
            return createMesh(mesh);
        }
    }
    std::cerr << "Mesh with name: " << name << " not found" << std::endl;

    Mesh3D mesh;
    return mesh;
}

std::vector<Mesh3D> MeshManager::getAllModels(){
    std::vector<Mesh3D> meshes;

    for(auto& mesh : cachedMeshes){
        meshes.push_back(createMesh(mesh));
    }

    return meshes;
}

AssimpMesh MeshManager::getModelData(const std::string& name){
    for(auto& mesh : cachedMeshes){
        if(mesh.name == name){
            return mesh;
        }
    }
    std::cerr << "Mesh with name: " << name << " not found" << std::endl;
    AssimpMesh mesh;
    return mesh;
}

std::vector<AssimpMesh> MeshManager::getAllModelData(){
    return cachedMeshes;
}

Mesh3D MeshManager::createMesh(AssimpMesh& mesh){
    Mesh3D returnMesh;

    std::vector<GLuint> indices = mesh.indices;
    std::vector<GLfloat> vertices;
    for(unsigned int i = 0; i < mesh.vertices.size() / 3; i++){
        vertices.push_back(mesh.vertices[i*3+0]);
        vertices.push_back(mesh.vertices[i*3+1]);
        vertices.push_back(mesh.vertices[i*3+2]);

        vertices.push_back(mesh.normals[i*3+0]);
        vertices.push_back(mesh.normals[i*3+1]);
        vertices.push_back(mesh.normals[i*3+2]);

        vertices.push_back(mesh.texCoords[i*2+0]);
        vertices.push_back(mesh.texCoords[i*2+1]);
    }
    glGenVertexArrays(1, &returnMesh.VAO);
    glBindVertexArray(returnMesh.VAO);

    glGenBuffers(1, &returnMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, returnMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // vvv Verticies vvv
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)0);
    // vvv Normals vvv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    // vvv UV Coordinates vvv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    glGenBuffers(1, &returnMesh.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


    glBindVertexArray(0);

    returnMesh.indexSize = indices.size();
    returnMesh.name = mesh.name;

    returnMesh.id = lastMesh;
    meshStorage[lastMesh] = returnMesh;
    lastMesh++;

    return returnMesh;
}








std::vector<uint32_t> MeshManager::parseObj(const std::string& location){
    meshLoader.loadModel(location);
    std::vector<std::pair<cpuMesh, std::string>> tempMeshes = meshLoader.extractCPUMeshes();
    std::vector<uint32_t> returnIDs;

    // extractMeshes() only returns meshes from last loadModel call, that is why I push it into the cache
    for(auto& tempMesh : tempMeshes){
        cachedCPUMeshes[lastCpuID] = tempMesh.first;
        
        meshInfo info;
        info.cpuMeshID = lastCpuID;
        info.displayName = tempMesh.second;
        info.path = location;

        std::stringstream ss;
        ss  << tempMesh.second
            << std::setw(4)
            << std::setfill('0')
            << lastMeshInfoID;
        info.internalName = ss.str();
        meshInfoObjects[lastMeshInfoID] = info;

        std::cout << "> Internal name: " << info.internalName << std::endl;
        std::cout << "> Last CPU ID: " << lastCpuID << std::endl;

        returnIDs.push_back(lastMeshInfoID);
        
        lastCpuID++;
        lastMeshInfoID++;
    }

    return returnIDs;
}

uint32_t MeshManager::compileMesh(uint32_t ID){
    gpuMesh gpuMesh;

    cpuMesh& cpuMesh = cachedCPUMeshes[meshInfoObjects[ID].cpuMeshID];

    std::vector<GLfloat> vertices;
    vertices.reserve(cpuMesh.vertices.size() / 3 * 8); // Tiny bit of a performance improvement, no longer needs to jump addresses.
    for(unsigned int i = 0; i < cpuMesh.vertices.size() / 3; i++){
        vertices.push_back(cpuMesh.vertices[i*3+0]);
        vertices.push_back(cpuMesh.vertices[i*3+1]);
        vertices.push_back(cpuMesh.vertices[i*3+2]);

        vertices.push_back(cpuMesh.normals[i*3+0]);
        vertices.push_back(cpuMesh.normals[i*3+1]);
        vertices.push_back(cpuMesh.normals[i*3+2]);

        vertices.push_back(cpuMesh.texCoords[i*2+0]);
        vertices.push_back(cpuMesh.texCoords[i*2+1]);
    }
    glGenVertexArrays(1, &gpuMesh.VAO);
    glBindVertexArray(gpuMesh.VAO);

    glGenBuffers(1, &gpuMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, gpuMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // vvv Verticies vvv
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)0);
    // vvv Normals vvv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    // vvv UV Coordinates vvv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    glGenBuffers(1, &gpuMesh.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cpuMesh.indices.size() * sizeof(GLuint), cpuMesh.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    gpuMesh.indexSize = cpuMesh.indices.size();

    cachedGPUMeshes[lastGpuID] = gpuMesh;
    meshInfoObjects[ID].gpuMeshID = lastGpuID;
    lastGpuID++;

    std::cout << "> Compiled mesh: " << meshInfoObjects[ID].internalName << " to GPU mesh!\n";

    return lastGpuID - 1;
}

gpuMesh& MeshManager::getGPUMesh(uint32_t id){
    auto it = meshInfoObjects.find(id);
    if(it == meshInfoObjects.end()){
        std::cerr << "Mesh info with ID: " << id << " not found!" << std::endl;
        static gpuMesh emptyMesh;
        return emptyMesh;
    }

    uint32_t gpuMeshID = it->second.gpuMeshID.value();
    gpuMesh& mesh = cachedGPUMeshes.at(gpuMeshID);
    return mesh;
}

meshInfo& MeshManager::getMeshInfo(uint32_t id){
    auto it = meshInfoObjects.find(id);
    if(it == meshInfoObjects.end()){
        std::cerr << "Mesh with ID: " << id << " not found!" << std::endl;
        static meshInfo emptyInfo;
        return emptyInfo;
    }

    return it->second;
}

std::vector<meshInfo*> MeshManager::getAllMeshInfo(){
    std::vector<meshInfo*> returnVector;
    for(auto& meshInfoObj : meshInfoObjects){
        returnVector.push_back(&meshInfoObj.second);
    }
    return returnVector;
}