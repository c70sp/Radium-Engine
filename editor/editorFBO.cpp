#include "editor.hpp"

void Editor::createFBO(){
    int width = 1920;
    int height = 1200;

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glGenTextures(1, &mColorTex);
    glBindTexture(GL_TEXTURE_2D, mColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTex, 0);

    glGenRenderbuffers(1, &mDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}