#include "./editor.hpp"

/**
 * Creates event listners for:
 * - Mouse Buttons
 * - Mouse Movement
 * - Mouse Scrolling
 * - Key Inputs
 */
void Editor::createEventListeners(){
    ENGINE->getEventSystem().subscribe<MouseButtonEvent>([this](const MouseButtonEvent& e){
        handleMouseButton(e);
    });
    ENGINE->getEventSystem().subscribe<MouseMoveEvent>([this](const MouseMoveEvent& e){
        handleMouseMove(e);
    });
    ENGINE->getEventSystem().subscribe<MouseScrollEvent>([this](const MouseScrollEvent& e){
        handleMouseScroll(e);
    });
    ENGINE->getEventSystem().subscribe<KeyPressEvent>([this](const KeyPressEvent& e){
        handleKeyPress(e);
    });
}

/**
 * This function runs as long as viewportDragActive (contained in editorUI) is true.
 * viewportDragActive is true when the the RMB gets pressed and held while hovering over the viewport.
 * First part is the basic camera translation on all axis.
 * Second part is the rotation of the camera on the yaw and pitch axis.
 * Third part is for changing the camera FOV.
 */
void Editor::handleViewportInputs(){
    auto cam = ENGINE->getCameraManager().getCamera("viewportCamera");
    // vvvvvvvvvv Translate vvvvvvvvvv

    float moveSpeed = 0.1;
    float shiftMultiplier = 10;

    float appliedSpeed = moveSpeed;
    if(keyData.heldButtons.contains(GLFW_KEY_LEFT_SHIFT)) appliedSpeed *= shiftMultiplier;

    if(keyData.heldButtons.contains(GLFW_KEY_W))
        cam->translate(glm::vec3(0, 0, 1), appliedSpeed);
    if(keyData.heldButtons.contains(GLFW_KEY_S))
        cam->translate(glm::vec3(0, 0, -1), appliedSpeed);
    if(keyData.heldButtons.contains(GLFW_KEY_A))
        cam->translate(glm::vec3(-1, 0, 0), appliedSpeed);
    if(keyData.heldButtons.contains(GLFW_KEY_D))
        cam->translate(glm::vec3(1, 0, 0), appliedSpeed);
    if(keyData.heldButtons.contains(GLFW_KEY_Q))
        cam->translate(glm::vec3(0, -1, 0), appliedSpeed);
    if(keyData.heldButtons.contains(GLFW_KEY_E))
        cam->translate(glm::vec3(0, 1, 0), appliedSpeed);

    // vvvvvvvvvv Rotate vvvvvvvvvv

    float sens = 0.1;

    cam->yaw += mouseData.delta.x * sens;
    cam->pitch -= mouseData.delta.y * sens;

    // vvvvvvvvvv FOV vvvvvvvvvv

    float fovsens = 2;

    if(mouseData.offset.y > 0.01) cam->fov -= mouseData.offset.y * fovsens;
    if(mouseData.offset.y < -0.01) cam->fov -= mouseData.offset.y * fovsens;
}

/**
 * Adds and removes mouse clicks from the mouseData.heldKeys set.
 * When the RMB is pressed while inside the editor viewport, the viewportDragActive flag gets set to true;
 * when released it gets set to false.
 * Also disabled the mouse cursor when viewportDragActive is true.
 */
void Editor::handleMouseButton(const MouseButtonEvent& e){
    if(e.action == GLFW_PRESS){
        mouseData.heldButtons.insert(GLFW_MOUSE_BUTTON_RIGHT);

        if(editorUI.mouseInViewport){ // Only checking mouseInViewport on press! Important to not get weird shit happening
            editorUI.viewportDragActive = true;
            glfwSetInputMode(ENGINE->window.mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }else if(e.action == GLFW_RELEASE){
        mouseData.heldButtons.erase(GLFW_MOUSE_BUTTON_RIGHT);
        editorUI.viewportDragActive = false;
        glfwSetInputMode(ENGINE->window.mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

/**
 * Updates: - current mouse pos,
 *          - last mouse pos,
 *          - delta mouse movement
 */
void Editor::handleMouseMove(const MouseMoveEvent& e){
    mouseData.lastMousePos = mouseData.mousePos;
    mouseData.mousePos = glm::vec2(e.xpos, e.ypos);
    mouseData.delta = mouseData.mousePos - mouseData.lastMousePos;
}

/**
 * Updates the mouse scolling offset.
 * In most cases the Y offset is all that is needed for scrolling.
 */
void Editor::handleMouseScroll(const MouseScrollEvent& e){
    mouseData.offset.x = e.xoffset;
    mouseData.offset.y = e.yoffset;
}

/**
 * Adds and removes pressed keys from the keyData.heldButtons set.
 * Also closes the editor when pressing the escape key.
 */
void Editor::handleKeyPress(const KeyPressEvent& e){
    if(e.action == GLFW_PRESS) keyData.heldButtons.insert(e.key);
    if(e.action == GLFW_RELEASE) keyData.heldButtons.erase(e.key);

    if(e.key == GLFW_KEY_ESCAPE && e.action == GLFW_PRESS) glfwSetWindowShouldClose(mEngine.window.mWindow, true);
};