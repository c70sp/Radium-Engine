#include "uiElement.hpp"

void UIElement::addChild(std::shared_ptr<UIElement> child){
    if(child->parent){
        child->parent->removeChild(child.get());
    }

    child->parent = this;
    children.push_back(child);
    child->onTransformChanged(); // Update child's position
}

void UIElement::removeChild(UIElement* child){
    children.erase(
        std::remove_if(children.begin(), children.end(), [child](const std::shared_ptr<UIElement>& element){
            return element.get() == child;
        }), children.end()
    );
}

void UIElement::clearChildren(){
    for(auto& child : children){
        child->parent = nullptr;
    }
    children.clear();
}

glm::vec2 UIElement::getAbsolutePosition() const{
    if(parent){
        return parent->getAbsolutePosition() + position;
    }
    return position;
}

void UIElement::setPosition(glm::vec2 newPos){
    position = newPos;
    onTransformChanged();
}

void UIElement::setSize(glm::vec2 newSize){
    size = newSize;
    onTransformChanged();
}

void UIElement::onTransformChanged(){
    updateGeometry();

    for(auto& child : children){
        child->onTransformChanged();
    }
}

void UIElement::draw(){
    if(!visible) return;

    // Draw this element (implemented in derived classes)
    // Will be overridden by UIRect::draw() for example

    for(auto& child : children){
        child->draw();
    }
}