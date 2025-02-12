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
#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"
#include "./../include/reactor.hpp"

// vvvvvvvvvv Globals vvvvvvvvvv

App gApp;
Reactor gReactor;

// ^^^^^^^^^^ Globals ^^^^^^^^^^
// vvvvvvvvvv Main function vvvvvvvvvv

int main(){
    gApp.initProgram();

    gReactor.init(&gApp);

    /**
     * Maybe do something like initProgram(main loop function name from this file) to have the main loop in the main.cpp file?
     * Would make life a bit easier, since after all, the app instance should be back end,
     * so everything happening on screen, aka everything important happens here, in the main.cpp file.
     */

    gReactor.mainLoop();

    gApp.cleanUp();
    return 0;
}