#pragma once
#include "uiManager/uiElement.hpp"

class UIRect : public UIElement{
    public:
        GLuint texID = 0;

        UIRect(glm::vec2 pos, glm::vec2 size, glm::vec4 color, GLuint prog) : UIElement(pos, size, color, prog) {};

        void init();
        void updateGeometry();
        void createBuffers();
        std::vector<GLfloat> genVertexData();
        std::vector<GLuint> genIndexData();

        // vvvvv Theoretically could use this in some scenario, though not sure if I will ever need it
        void draw() override;
};