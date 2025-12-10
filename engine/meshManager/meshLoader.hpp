#pragma once

// C++ STL
#include <vector>
#include <iostream>

// 3rd party libs
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Own libs
#include "mesh3d.hpp"

class MeshLoader{
    public:
        bool loadModel(const std::string& filepath);
        std::vector<AssimpMesh> extractMeshes();
    private:
        Assimp::Importer importer;
        const aiScene* scene;

        void printMeshInfo(const AssimpMesh& mesh, int meshIndex);
        void getMeshInfo();
};