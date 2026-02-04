#include "renderer.hpp"

void Renderer::beginFrame(){
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    //? vvvvv not sure if I should make this seperate for just UI, as it could potentially slow down the prog. if applied to everything
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame(){
    // can handle post processing and UI later
}

GLenum Renderer::getGLIndexType(IndexType type){
    switch(type){
        case IndexType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
        case IndexType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
        case IndexType::UNSIGNED_INT: return GL_UNSIGNED_INT;
    }
    return GL_UNSIGNED_INT;
}

void Renderer::drawMesh(const Mesh3D& mesh, ShaderManager& sm, const glm::mat4 VPMatrix){
    glUseProgram(mesh.program);
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(currentProgram == 0) std::cerr << "Current program: " << currentProgram << ", probably means shit's fucked\n";
    
    if(mesh.hasTexture){
        sm.setUniformTex2D(mesh.program, "u_texture", mesh.texture->id, GL_TEXTURE0);
    }
    glBindVertexArray(mesh.VAO);

    sm.setUniformMat4(mesh.program, "u_ModelMatrix", mesh.transforms);
    sm.setUniformMat4(mesh.program, "u_VP", VPMatrix);

    GLenum glType = getGLIndexType(mesh.indexType); // Just for safety
    glDrawElements(GL_TRIANGLES, mesh.indexSize, glType, NULL);

    glUseProgram(0);
}

glm::mat4 Renderer::createModelMatrix(TransformComponent& tc){
    glm::mat4 mm = glm::mat4(1.0f);

    mm = glm::translate(mm, tc.pos);
    mm = glm::rotate(mm, glm::radians(tc.rot.x), glm::vec3(1, 0, 0));
    mm = glm::rotate(mm, glm::radians(tc.rot.y), glm::vec3(0, 1, 0));
    mm = glm::rotate(mm, glm::radians(tc.rot.z), glm::vec3(0, 0, 1));
    mm = glm::scale(mm, tc.scale);

    return mm;
}

void Renderer::drawMesh2(const Mesh3D& mesh, TransformComponent& tc, MaterialComponent& mc, ShaderManager& sm, glm::mat4& VPMatrix){
    glUseProgram(mc.programID);
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(currentProgram == 0) std::cerr << "Current program: " << currentProgram << ", probably means shit's fucked\n";

    glBindVertexArray(mesh.VAO);

    glm::mat4 mm = createModelMatrix(tc);
    sm.setUniformMat4(mc.programID, "u_ModelMatrix", mm);

    sm.setUniformMat4(mc.programID, "u_VP", VPMatrix);

    GLenum glType = getGLIndexType(mesh.indexType);
    glDrawElements(GL_TRIANGLES, mesh.indexSize, glType, NULL);

    glUseProgram(0);
}

void Renderer::drawMeshNewSystem(const std::vector<Renderable>& renderQueue, const glm::mat4& VPMatrix){
    // std::cout << "Yo, render queue is here: " << renderQueue.size() << std::endl;

    for(auto& e : renderQueue){
        glUseProgram(e.mat->programID);
        GLint currentProg = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
        if(currentProg == 0) std::cerr << "Current prog: " << currentProg << ", prob. means shit's fucked\n";

        auto& mesh = meshManagerRef.getGPUMesh(e.mesh->meshInfoID);

        glBindVertexArray(mesh.VAO);
        glm::mat4 mm = createModelMatrix(*e.trans);
        shaderManagerRef.setUniformMat4(e.mat->programID, "u_ModelMatrix", mm);

        shaderManagerRef.setUniformMat4(e.mat->programID, "u_VP", VPMatrix);

        GLenum glType = getGLIndexType(IndexType::UNSIGNED_INT);
        glDrawElements(GL_TRIANGLES, mesh.indexSize, glType, NULL);

        glUseProgram(0);
    }
}

void Renderer::drawUI(UIManager& ui){
    ui.drawAll();
}