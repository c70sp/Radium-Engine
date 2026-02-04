#pragma once

// C++ STL
#include <iostream>

// 3rd party libs
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own libs
#include "./../meshManager/mesh3d.hpp"
#include "./../meshManager/meshManager.hpp"
#include "./../shaderManager/shaderManager.hpp"
#include "./../../scene/sceneStruts.hpp"
#include "./../uiManager/uiManager.hpp"

class ShaderManager;
class MeshManager;

class Renderer{
    public:
        Renderer(MeshManager& mm, ShaderManager& sm) : meshManagerRef(mm), shaderManagerRef(sm) {};

        void beginFrame();
        void endFrame();

        // void setMeshManagerRef(MeshManager& mm) { meshManagerRef = &mm; }
        // void setShaderManagerRef(ShaderManager& sm) { shaderManagerRef = &sm; }

        glm::mat4 createModelMatrix(TransformComponent& tc);

        void drawMesh(const Mesh3D& mesh, ShaderManager& sm, const glm::mat4 VPMatrix);
        void drawMesh2(const Mesh3D& mesh, TransformComponent& tc, MaterialComponent& mc, ShaderManager& sm, glm::mat4& VPMatrix);

        void drawMeshNewSystem(const std::vector<Renderable>& renderQueue, const glm::mat4& VPMatrix);
        void drawSingleObject(); // Same shit as drawMeshNewSystem, but without sending an entire render queue.
        
        void drawUI(UIManager& ui);

    private:
        MeshManager& meshManagerRef;
        ShaderManager& shaderManagerRef;

        GLenum getGLIndexType(IndexType type);
};