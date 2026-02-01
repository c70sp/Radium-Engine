#pragma once

#include <iostream>

#include "./../engine/engine.hpp"
#include "./../scene/sceneManager.hpp"
#include "./editorUI/editorUI.hpp"

#include "./../staticConfig.hpp"

class Editor{
    public:
        Editor(Engine& engine, SceneManager& sceneManager);
        void init();
        void update(float dt);
        void render();
        void shutdown();

    private:
        SceneManager& mSceneManager;
        Engine& mEngine;

        EditorUI editorUI;

        GLuint mFBO, mColorTex, mDepthRBO;

        Scene* mScene1;
        // SceneStruct scene1;
        // Entity* ent;


        void createFBO();
        void drawAllToFBO();
};