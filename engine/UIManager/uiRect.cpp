#include "uiManager/uiRect.hpp"

std::vector<GLfloat> UIRect::genVertexData(){
    float x = position.x;
    float y = position.y;

    std::vector<GLfloat> vertices = {
        0.0f + x, 0.0f + y, 0.0f, 0.0f, // BL
        size.x + x, 0.0f + y, 1.0f, 0.0f, // BR
        size.x + x, size.y + y, 1.0f, 1.0f, // TR
        0.0f + x, size.y + y, 0.0f, 1.0f // TL
    };

    return vertices;
}

std::vector<GLuint> UIRect::genIndexData(){
    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3
    };

    return indices;
}

void UIRect::createBuffers(){
    std::vector<GLfloat> vertices = genVertexData();
    std::vector<GLuint> indices = genIndexData();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    indexSize = indices.size();
}

void UIRect::init(){
    createBuffers();
}

void UIRect::updateGeometry(){
    std::vector<GLfloat> vertices = genVertexData();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
}

void UIRect::draw(){
    if(!visible) return;

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, nullptr);

    // Draw children (automatically called by base class)
    UIElement::draw();
}