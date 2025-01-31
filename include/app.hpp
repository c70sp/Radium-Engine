#ifndef APP_HPP
#define APP_HPP

#define GLM_ENABLE_EXPERIMENTAL

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set> // for keeping track of "divs"
#include <memory>

// Own libs
#include "./../include/mesh3d.hpp"
#include "./../include/shader.hpp"
#include "./../include/structs.hpp"
class Mesh3D; // forward declaration

class App{
    public:
        //* vvvvvvvvvv Attributes vvvvvvvvvv

        GLFWwindow* mWindow;
        int mScreenWidth = 640;
        int mScreenHeight = 480;

        std::vector<std::shared_ptr<Mesh3D>> mMeshes{};
        std::vector<Shader> mShaders{};

        std::vector<std::shared_ptr<Mesh3D>> mDrawQueue{};

        Shader mShader;

        //* ^^^^^^^^^^ Attributes ^^^^^^^^^^
        //* vvvvvvvvvv Methodes vvvvvvvvvv

        // Destructor
        ~App();

        //* Main functions
        // Get all sorts of GPU info
        void getOpenGLVersionInfo();
        
        // Start the program up
        int initProgram();
        
        // Set up the OpenGL thingys
        void preDraw();

        /**
         * Main loop in reactor.cpp pushes neutrons and atoms into the drawQueue here,
         * then calls this draw, which first calls preDraw and then renders objects sequentially from the drawQueue.
         * Clears the drawQueue.
         */
        void draw();

        // Deletes all shaders and meshes
        void cleanUp();

        // Collision check between two circles
        bool ccCollision(Circle circle1, Circle circle2);

        //? Mesh related stuff
        std::shared_ptr<Mesh3D> createRect(float x, float y, float w, float h);
        std::shared_ptr<Mesh3D> createCircle(float x, float y, float r, int res);

        //* ^^^^^^^^^^ Methodes ^^^^^^^^^^
    private:
        float mCurrentTime = 0.0f;
        float mLastFrame = 0.0f;
        float mDeltaTime = 0.0f;

        glm::mat4 mProjectionMatrix = glm::mat4(1.0f);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void handleKeyInput(int key, int scancode, int action, int mods);
};

#endif // APP_HPP