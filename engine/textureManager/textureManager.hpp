#pragma once

// C++ STL
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

// 3rd party lib
#include <glad/glad.h>
#include <glm/glm.hpp>
#include ".libs/stb_image.h"

// Own libs
#include "meshManager/mesh3d.hpp"

class TextureManager{
    public:
        TextureManager();
        ~TextureManager();

        std::shared_ptr<Texture> loadTexture(const std::string& filepath, const std::string& type = "diffuse", bool flipUVs = true);
        std::shared_ptr<Texture> createTexture(unsigned char* data, int width, int height, int channels = 4, const std::string& name = "");
        std::shared_ptr<Texture> getTexture(const std::string& filepath); // could also do name / function overloading for both

        bool isLoaded(const std::string& filepath);

        void cleanUp();
        
    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        
        GLuint createGLTexture(unsigned char* data, int width, int height, int channels);
        void setupTextureParameters(GLuint textureID);
        void debugTexture(GLuint textureID, const std::string& name);
};