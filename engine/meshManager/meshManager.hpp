#pragma once

// C++ STL
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <sstream>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Own libs
#include "mesh3d.hpp"
#include "meshLoader.hpp"
#include "./../core/common.hpp"
#include "./../../staticConfig.hpp"

class MeshManager{
    public:
        MeshLoader meshLoader;

        // Loads the obj file and parses it, to create an AssimpMesh
        void loadModel(const std::string& filepath);
        
        // Get a/all model(s) as a Mesh3D (OpenGL formats (VAO, VBO, etc.)) from the loaded model cache
        Mesh3D getModel(const std::string& name);
        std::vector<Mesh3D> getAllModels();

        // Returns the/all AssimpMesh(es)
        AssimpMesh getModelData(const std::string& name);
        std::vector<AssimpMesh> getAllModelData();

        // Contains OpenGL compatible meshes
        std::unordered_map<uint32_t, Mesh3D> meshStorage;








        std::vector<uint32_t> parseObj(const std::string& location);
        
        uint32_t compileMesh(uint32_t ID);
        uint32_t compileMesh(const std::string& name);

        gpuMesh& getGPUMesh(uint32_t id);
        meshInfo& getMeshInfo(uint32_t id);
        std::vector<meshInfo*> getAllMeshInfo();

        // std::unordered_map<uint32_t, meshInfo>& getAllMeshInfoObjects(){return meshInfoObjects;};
    private:
        uint32_t lastMesh = 0;

        // List of all AssimpMeshes, contains pure vertex, index, normal and texCoord data
        std::vector<AssimpMesh> cachedMeshes;
        // Turns an AssimpMesh into a Mesh3D (OpenGL compatible data)
        Mesh3D createMesh(AssimpMesh& mesh);










        std::unordered_map<uint32_t, cpuMesh> cachedCPUMeshes;
        std::unordered_map<uint32_t, gpuMesh> cachedGPUMeshes;
        std::unordered_map<uint32_t, meshInfo> meshInfoObjects;

        uint32_t lastCpuID = 0;
        uint32_t lastGpuID = 0;
        uint32_t lastMeshInfoID = 0;
};