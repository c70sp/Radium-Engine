#pragma once

// C++ STL
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

// 3rd party libs
#include <glad/glad.h>
#include <glm/glm.hpp>

#define FT2_BUILD_LIBRARY
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

// Own libs
#include "textureManager/textureManager.hpp"

struct Character{
    std::shared_ptr<Texture> texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};


class TextManager{
    public:
        TextManager(TextureManager& textureManager);
        ~TextManager();

        bool loadFont(const std::string& fontPath, GLuint programIn, int fontSize = 48);
        void renderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color = glm::vec3(1.0f));
        glm::vec2 getTextSize(const std::string& text, float scale);

        // vvvvvvvvvvvvvvv not used since it doesn't work :/ vvvvvvvvvvvvvvv
        // void setFontVariation(const std::string& axisName, float value);
        // void setFontWeight(float weight);
        // void setFontWidth(float width);

    private:
        // std::unordered_map<std::string, FT_Fixed> variationCoords;
        std::unordered_map<char, Character> characters;

        GLuint VAO, VBO;
        GLuint program;
        FT_Library ft;
        FT_Face face;
        TextureManager& textureManager;

        void setupBuffers();
};