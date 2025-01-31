#define GLM_ENABLE_EXPERIMENTAL

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <dwmapi.h>

// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set> // for keeping track of "divs"
#include <unordered_map>
#include <memory>

// Own libs
#include "./../include/utils.hpp"
#include "./../include/app.hpp"
#include "./../include/structs.hpp"

void App::getOpenGLVersionInfo(){
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int App::initProgram(){
    //* vvvvvvvvvv OPENGL init vvvvvvvvvv

    if(!glfwInit()){
        std::cerr << "Failed to init GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GL_DOUBLEBUFFER, 1);
    glfwWindowHint(GL_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 8); // Anti-aliasing, 8x MSAA (glaube ich)
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "Title", NULL, NULL);
    if(mWindow == NULL){
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init glad." << std::endl;
        glfwTerminate();
        return -1;
    }

    getOpenGLVersionInfo();
    // glfwSetErrorCallBack();

    /**
     * So, I need the user-pointer, because GLFW is C-Style,
     * the user-pointer basically just connects some information to a window,
     * in this case this instance with mWindow.
     * Thanks to the window now holding this class, when the static keyCallback gets called,
     * I can retrieve this instance by looking for a window with the user pointer to this instance.
     * If a window gets found, I can get a pointer to it and use that to call the actual keyCallback (handleKeyInput) of this instance.
     * https://chatgpt.com/c/67688520-5f18-8011-97da-b591b217712e
     */
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallback);

    glfwSetWindowPos(mWindow, 100, 100);

    /** 
     * Disabling DWM to remove stuttering
     * only worked a little (or not at all) lol, but why not keep it here, even if it just helps in some rare cases
     * If it even works on windows 10.. Win 7 allowed it, Win 8 did not.
     */
    HRESULT hr = DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
    if (!SUCCEEDED(hr)) {
        std::cerr << "Failed to disable DWM" << std::endl;
    }

    //* ^^^^^^^^^^ OPENGL init ^^^^^^^^^^
    //* vvvvvvvvvv App init vvvvvvvvvv

    // Compiles all shaders in the shader instance
    mShader.compileAllShaders();

    // Set perspective matrix
    // mProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)mScreenWidth/(float)mScreenHeight, 0.1f, 100.0f);
    mProjectionMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight, -100.0f, 100.0f);

    //* ^^^^^^^^^^ App init ^^^^^^^^^^
    return 0;
}

/**
 * Have to do this since GLFW is C-Style, so it does not support classes / instances.
 * Meaning I need this as a static methode, which retrieves the window memory location,
 * which then sends the input to the proper methode for handling keys.
 */
void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if(app){
        app->handleKeyInput(key, scancode, action, mods);
    }
}

void App::handleKeyInput(int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
    }
    if(key == GLFW_KEY_UP){
        // Zoom by changing ortho values
    }
}

void App::cleanUp(){
    glfwDestroyWindow(mWindow);

    for(std::shared_ptr<Mesh3D>& i : mMeshes){
        i->destroy();
    }

    glfwTerminate();
}

// App destructor, automatically runs when app gets destroyed.
App::~App(){
    std::cout << "Destructing App instance. Goodbye :(" << std::endl;
    if(mWindow){
        glfwDestroyWindow(mWindow);
        
        for(std::shared_ptr<Mesh3D>& i : mMeshes){
            i->destroy();
        }

        glfwTerminate;
    }
}

void App::preDraw(){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, mScreenWidth, mScreenHeight);
    glClearColor(0.10f, 0.10f, 0.10f, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void App::draw(){
    App::preDraw();

    for(const std::shared_ptr<Mesh3D>& i : mDrawQueue){
        i->meshDraw(mProjectionMatrix);
        utils::checkGLError("meshDraw");
    }

    mDrawQueue.clear();

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

bool App::ccCollision(Circle circle1, Circle circle2){
    float dist = utils::getDistance({circle1.x, circle1.y}, {circle2.x, circle2.y});

    if(dist <= circle1.r + circle2.r) return true;
    else return false;
}

// Create Meshes (VertexData and IBO)
std::shared_ptr<Mesh3D> App::createRect(float x, float y, float w, float h){
    std::cout << "Creating rectangle: " << x << " | " << y << " | " << w << " | " << h << std::endl;

    // Create mesh
    std::shared_ptr mesh = std::make_shared<Mesh3D>(this); // Send model matrix and VBO into constructor

    // Create coords
    const std::vector<GLfloat> vertexData{
        0.0f, 0.0f, 0.0f, // BL
        w, 0.0f, 0.0f, // BR
        0.0f, h, 0.0f, // TL
        w, h, 0.0f // TR
    };
    const std::vector<GLuint> indexBufferData{
        2, 0, 1,
        3, 2, 1
    };

    // Create model matrix for translation, rotation and scaling / stretching / skewing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, (float)mScreenHeight - y - h, 0.0f));

    // Set attribs
    mesh->setApp(this);
    mesh->setModelMatrix(model);
    mesh->setIndexBufferData(indexBufferData);
    mesh->setVertexData(vertexData);

    // Push into list
    return mesh;
}

std::shared_ptr<Mesh3D> App::createCircle(float x, float y, float r, int res) {
    GLfloat x2, y2;
    std::vector<GLfloat> vertexData{};
    std::vector<GLuint> indexBufferData{};
    std::shared_ptr<Mesh3D> mesh = std::make_shared<Mesh3D>(this);

    float deg = 360.0f / res;
    vertexData.push_back(0.0f);  // Center vertex
    vertexData.push_back(0.0f);
    vertexData.push_back(0.0f);

    for (int i = 0; i < res; ++i) {
        float angle = glm::radians(i * deg); // Using rad
        x2 = r * cos(angle);
        y2 = r * sin(angle);

        vertexData.push_back(x2);
        vertexData.push_back(y2);
        vertexData.push_back(0.0f);

        if (i < res - 1) {
            indexBufferData.push_back(i + 1);   // Current vertex
            indexBufferData.push_back(0);       // Center vertex
            indexBufferData.push_back(i + 2);   // Next vertex
        }else if(i < res){
            indexBufferData.push_back(i + 1);   // Current vertex
            indexBufferData.push_back(0);       // Center vertex
            indexBufferData.push_back(1);       // First vertex
        }
    }

    mesh->setApp(this);
    mesh->setType("circle");
    mesh->setVertexData(vertexData);
    mesh->setIndexBufferData(indexBufferData);

    mesh->mX = x;
    mesh->mY = y;

    return mesh;
}