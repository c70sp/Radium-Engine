#define STB_IMAGE_IMPLEMENTATION

#include "textureManager.hpp"

TextureManager::TextureManager(){
    stbi_set_flip_vertically_on_load(true); // default to flip UVs
}

TextureManager::~TextureManager(){
    cleanUp();
}

std::shared_ptr<Texture> TextureManager::loadTexture(const std::string& filepath, const std::string& type, bool flipUVs){
    if(auto it = textures.find(filepath); it != textures.end()){
        return it->second;
    }

    stbi_set_flip_vertically_on_load(flipUVs);

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if(!data){
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        std::cerr << "STBI error: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }
    std::cout << "STBI loaded: " << width << "x" << height << " with " << channels << " channels" << std::endl;

    GLuint textureID = createGLTexture(data, width, height, channels);

    stbi_image_free(data);

    if(textureID == 0){
        std::cerr << "Failed to create texture:" << filepath << std::endl;
        return nullptr;
    }

    auto texture = std::make_shared<Texture>(); // muss () sein?
    texture->id = textureID;
    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->type = type;
    texture->path = filepath;

    // store in unorderd_map cache
    textures[filepath] = texture;

    std::cout << "Loaded texture: " << filepath << " (" << width << "x" << height << ")" << std::endl;

    //?! vvvvv uncomment if shit decides to break, which it inevitably will, probably
    // debugTexture(textureID, "test");

    return texture;
}

std::shared_ptr<Texture> TextureManager::createTexture(unsigned char* data, int width, int height, int channels, const std::string& name){
    GLuint textureID = createGLTexture(data, width, height, channels);

    if(textureID == 0){
        std::cerr << "Failed to create texture: " << name << std::endl;
        return nullptr;
    }

    auto texture = std::make_shared<Texture>();
    texture->id = textureID;
    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->type = "generated";
    texture->path = name;

    if(!name.empty()){
        textures[name] = texture;
    }

    return texture;
}

GLuint TextureManager::createGLTexture(unsigned char* data, int width, int height, int channels){
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // std::cout << "glGenTextures returned ID: " << textureID << std::endl;
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error after glGenTextures: " << error << std::endl;
    }
    
    if (textureID == 0) {
        std::cout << "ERROR: glGenTextures failed - returned 0!" << std::endl;
        return 0;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    // std::cout << "Successfully bound texture ID: " << textureID << std::endl;

    GLenum internalFormat, format = GL_RGB;

    if(channels == 1){
        // std::cout << "Channels: " << channels << std::endl;
        format = GL_RED;
        internalFormat = GL_R8;
    }
    else if(channels == 3){
        // std::cout << "Channels: " << channels << std::endl;
        format = GL_RGB;
        internalFormat = GL_RGB8;
    }
    else if(channels == 4){
        // std::cout << "Channels: " << channels << std::endl;
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }
    else{
        std::cerr << "Unsupported amount of channels: " << channels << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // glGenerateMipmap(GL_TEXTURE_2D); // for mipmaps, disabled because currently only using 8x8 palette which sucks with mipmaps

    setupTextureParameters(textureID);

    return textureID;
}

void TextureManager::setupTextureParameters(GLuint textureID){
    glBindTexture(GL_TEXTURE_2D, textureID);

    // wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // filtering
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // GL_LINEAR_MIPMAP_NEAREST, used for mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR

    // Anisotropic filtering (if supported ig)
    if(GLAD_GL_EXT_texture_filter_anisotropic){
        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& filepath){
    auto it = textures.find(filepath);
    if(it != textures.end()){
        return it->second;
    }else{
        std::cerr << "File not found: " << filepath << std::endl;
        return nullptr;
    }
}

bool TextureManager::isLoaded(const std::string& filepath){
    return textures.find(filepath) != textures.end();
}

void TextureManager::cleanUp(){
    for(auto& [path, texture] : textures){
        // std::cout << "Deleting textureID: " << texture->id << std::endl;
        glDeleteTextures(1, &texture->id);
    }
    textures.clear();
}

void TextureManager::debugTexture(GLuint textureID, const std::string& name){
    std::cout << "Texture '" << name << "' ID: " << textureID << std::endl;
    
    // Check if OpenGL thinks this is a valid texture
    if (glIsTexture(textureID)) {
        std::cout << "Texture is valid" << std::endl;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLint width, height;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        std::cout << "Size: " << width << "x" << height << std::endl;
    } else {
        std::cerr << "Texture is invalid" << std::endl;
    }
}