#include "editor.hpp"

/**
 * Creates a basic FBO.
 * FBO contains an 8-bit color texture and a render / depth buffer.
 * Quickly checks if it is broken at the end.
 */
void Editor::createFBO(){
    int width = editorUI.viewportSize.x;
    int height = editorUI.viewportSize.y;

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glGenTextures(1, &mColorTex);
    glBindTexture(GL_TEXTURE_2D, mColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTex, 0);

    glGenRenderbuffers(1, &mDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Quick error logging
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    if (staticConfig::errorLogging && glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer broken!!\n";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * Resizes the FBO color texture and render / depth buffer.
 * This is done so that the viewport can be resized properly without breaking.
 */
void Editor::resizeFBO(){
    int width = editorUI.viewportSize.x;
    int height = editorUI.viewportSize.y;

    glBindTexture(GL_TEXTURE_2D, mColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

/**
 * Recalculated the aspect ratio of the viewport camera on resize.
 * If not done, the render will look weird.
 */
void Editor::recalcCameraAspect(){
    float newAspect = static_cast<float>(editorUI.viewportSize.x) / static_cast<float>(editorUI.viewportSize.y);
    ENGINE->getCameraManager().getCamera("viewportCamera")->aspect = newAspect;
}

void Editor::resizeViewport(){
    resizeFBO();
    recalcCameraAspect();
    editorUI.viewportChanged = false;
}