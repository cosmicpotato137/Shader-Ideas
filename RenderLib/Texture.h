#pragma once
#include "core.h"

#include <GL/glew.h>
#include <iostream>
#include <string>

class Texture {
    GLuint textureID;
    int width;
    int height;
public:
    Texture() : textureID(0), width(0), height(0) {}
    ~Texture() { Cleanup(); }

    // Initialize the texture from an image file
    bool Init(const std::string& filename);
    bool Init(int width, int height);

    // Bind the texture for rendering
    void Bind() const;
    void BindCompute() const;

    // Save texture to image
    bool SaveToImage(const std::string& filePath);

    // Unbind the currently bound texture
    void Unbind() const;

    // Get the texture width
    int GetWidth() const { return width; }

    // Get the texture height
    int GetHeight() const { return height; }

    // Get the texture ID
    int GetTextureID() { return textureID; }

    // Clean up and release resources
    void Cleanup();
};
