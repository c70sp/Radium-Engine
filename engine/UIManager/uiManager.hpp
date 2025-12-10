#pragma once

// C++ STL
#include <vector>
#include <iostream>
#include <memory>

// 3rd party lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Own libs
#include "uiManager/uiElement.hpp"
#include "uiManager/uiRect.hpp"

class UIManager{
    public:
        std::vector<std::shared_ptr<UIElement>> elements;

        template<typename T, typename... Args>
        std::shared_ptr<T> create(Args&&... args){
            auto element = std::make_shared<T>(std::forward<Args>(args)...);
            element->init();
            elements.push_back(element);
            return element;
        }

        std::vector<std::shared_ptr<UIElement>> getElements(){
            return elements;
        };

        void updateAllGeometries(){
            for(auto& e : elements){
                e->updateGeometry();
            }
        }

        void drawAll(){
            for(auto& e : elements){
                e->draw();
            }
        }

    private:
};