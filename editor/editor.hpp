#pragma once

#include <iostream>

#include "./../engine/engine.hpp"
#include "./../scene/scene.hpp"
#include "./editorUI/editorUI.hpp"

class Editor{
    public:
        Editor(Engine& engine, Scene& scene);
        void init();
        void update(float dt);
        void render();
        void shutdown();

    private:
        Scene& mScene;
        Engine& mEngine;

        EditorUI editorUI;

        GLuint mFBO, mColorTex, mDepthRBO;

        SceneStruct scene1;
        Entity* ent;


        void createFBO();
        void drawAllToFBO();
};