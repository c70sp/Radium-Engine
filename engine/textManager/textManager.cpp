#include "textManager.hpp"

TextManager::TextManager(TextureManager& textureManager) : textureManager(textureManager){
    // std::cout << "FreeType version: " << FREETYPE_MAJOR << "." << FREETYPE_MINOR << "." << FREETYPE_PATCH << std::endl;
    if(FT_Init_FreeType(&ft)){
        std::cerr << "Failed to init freetype" << std::endl;
    }
}

TextManager::~TextManager(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    if(face){
        FT_Done_Face(face);
    }
    FT_Done_FreeType(ft);
}

bool TextManager::loadFont(const std::string& fontPath, GLuint programIn, int fontSize){
    if(FT_New_Face(ft, fontPath.c_str(), 0, &face)){
        std::cerr << "Failed to load font: " << fontPath << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    program = programIn;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0; c < 128; c++){
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cerr << "Failed to load glyph: " << c << std::endl;
            continue;
        }

        std::string textureName = "font_glyph_" + std::to_string(static_cast<int>(c));

        auto glyphTexture = textureManager.createTexture(
            face->glyph->bitmap.buffer,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            1,
            textureName
        );

        if(!glyphTexture){
            std::cerr << "Failed to create texture for glypth: " << c << std::endl;
            continue;
        }

        // TextureManager defaults to GL_REPEAT, so I overwrite it here
        glBindTexture(GL_TEXTURE_2D, glyphTexture->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Character character = {
            glyphTexture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        characters[c] = character;
    }

    setupBuffers();
    std::cout << "Loaded font: " << fontPath << " with " << characters.size() << " characters" << std::endl;
    return true;
}

void TextManager::setupBuffers(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindVertexArray(0);
}

void TextManager::renderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color){
    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "u_textColor"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for(const char& c : text){
        Character& ch = characters[c];

        GLfloat xpos = position.x + ch.bearing.x * scale;
        GLfloat ypos = position.y - (ch.size.y - ch.bearing.y) * scale;
        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[6][4] = {
            {xpos,      ypos + h,   0.0f, 0.0f},
            {xpos,      ypos,       0.0f, 1.0f},
            {xpos + w,  ypos,       1.0f, 1.0f},
            {xpos,      ypos + h,   0.0f, 0.0f},
            {xpos + w,  ypos,       1.0f, 1.0f},
            {xpos + w,  ypos + h,   1.0f, 0.0f},
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture->id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        position.x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 TextManager::getTextSize(const std::string& text, float scale){
    float width = 0.0f;
    float maxHeight = 0.0f;

    for(const char& c : text){
        Character& ch = characters[c];
        width += (ch.advance >> 6) * scale;
        maxHeight = std::max(maxHeight, (float)ch.size.y * scale);
    }

    return glm::vec2(width, maxHeight);
}

// vvvvvvvvvvvvvvv shit doesn't work, idk man vvvvvvvvvvvvvvv
// void TextManager::setFontVariation(const std::string& axisName, float value){
//     if(!FT_HAS_MULTIPLE_MASTERS(face)){
//         std::cerr << "Font is not variable" << std::endl;
//         return;
//     }

//     FT_Fixed fixedValue = static_cast<FT_Fixed>(value * 65536.0f);
//     variationCoords[axisName] = fixedValue;

//     // Fucking stupid implementation, should not use an unordered_map. Fuck it, future me problem.
//     std::vector<FT_Fixed> coords;
//     for (const auto& pair : variationCoords) {
//         coords.push_back(pair.second);
//     }
//     FT_Set_Var_Design_Coordinates(face, coords.size(), coords.data());
// }

// void TextManager::setFontWeight(float weight){
//     setFontVariation("wght", weight);
// }

// void TextManager::setFontWidth(float width){
//     setFontVariation("wdth", width);
// }