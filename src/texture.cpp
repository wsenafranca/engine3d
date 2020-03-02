//
// Created by Washington on 21/02/2020.
//

#include "texture.hpp"
#include "opengl.hpp"

Texture::Texture(uint32_t target) : mTarget(target) {
    glGenTextures(1, &glTextureId);
    glBindTexture(target, glTextureId);

    SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    SetParameter(GL_TEXTURE_WRAP_R, GL_REPEAT);

    glBindTexture(target, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &glTextureId);
}

void Texture::SetBitmap(const Bitmap& bitmap) {
    SetBitmap(mTarget, bitmap);
}

void Texture::SetBitmap(uint32_t bitmapTarget, const Bitmap &bitmap) {
    mWidth = bitmap.GetWidth();
    mHeight = bitmap.GetHeight();
    mFormat = bitmap.GetFormat();

    glTexImage2D(
            bitmapTarget,
            0,
            bitmap.GetInternalFormat(),
            mWidth,
            mHeight,
            0,
            mFormat,
            bitmap.GetType(),
            bitmap.GetData());
}

void Texture::GenerateMipmap() {
    glGenerateMipmap(mTarget);
    SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

void Texture::Bind() const {
    glBindTexture(mTarget, glTextureId);
}

void Texture::BindActive(int textureSlot) const {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    Bind();
}

void Texture::Unbind() const {
    glBindTexture(mTarget, 0);
}

void Texture::SetParameter(uint32_t pname, int32_t param) {
    glTexParameteri(mTarget, pname, param);
}

void Texture::SetParameter(uint32_t pname, float param) {
    glTexParameterf(mTarget, pname, param);
}

uint32_t Texture::GetTarget() const {
    return mTarget;
}

uint32_t Texture::GetWidth() const {
    return mWidth;
}

uint32_t Texture::GetHeight() const {
    return mHeight;
}

uint32_t Texture::GetFormat() const {
    return mFormat;
}
