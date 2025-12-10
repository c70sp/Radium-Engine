#pragma once

// C++ STL
#include <vector>
#include <iostream>
#include <memory>

// 3rd party lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class UIElement{
    public:
        glm::vec2 position; // Relative to parent
        glm::vec2 size;
        float radius;
        glm::vec4 color;
        bool visible = true;

        // glm::mat4 transforms = glm::mat4(1.0f); Not needed for now, since I only got a rect

        GLuint VAO, VBO, EBO, norm;
        int indexSize;
        GLuint program;

        // Child management
        std::vector<std::shared_ptr<UIElement>> children;
        UIElement* parent = nullptr;

        UIElement(glm::vec2 pos, glm::vec2 size, glm::vec4 color, GLuint prog) : position(pos), size(size), color(color), program(prog) {};
        UIElement(glm::vec2 pos, float radius, glm::vec4 color, GLuint prog) : position(pos), radius(radius), color(color), program(prog) {};

        void addChild(std::shared_ptr<UIElement> child);
        void removeChild(UIElement* child);
        void clearChildren();

        // Parent-relative (so this element's pos) + parents absolute pos
        glm::vec2 getAbsolutePosition() const;

        void setPosition(glm::vec2 newPos);
        void setSize(glm::vec2 newSize);

        
        virtual void init() = 0;
        virtual void updateGeometry() = 0;
        virtual void draw() = 0;

        virtual ~UIElement() {};

    protected:
        virtual void onTransformChanged();
};