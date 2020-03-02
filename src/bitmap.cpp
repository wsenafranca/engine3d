//
// Created by Washington on 25/02/2020.
//

#include "bitmap.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "opengl.hpp"

Bitmap::Bitmap()
        : mWidth(0), mHeight(0), mFormat(GL_RGB), mInternalFormat(GL_RGB), mType(GL_UNSIGNED_BYTE) {

}

Bitmap::Bitmap(uint8_t *data, uint32_t width, uint32_t height, uint32_t format, uint32_t type)
        : mWidth(width), mHeight(height), mFormat(format), mInternalFormat(format), mType(type)
{
    mData.reset(data);
}

Bitmap::Bitmap(const filesystem::path &file) {
    if(!file.exists()) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
    }

    int w, h, n;
    mData.reset(stbi_load(file.str().c_str(), &w, &h, &n, 0));
    if(!mData) {
        throw std::runtime_error("Error reading image " + file.str());
    }

    switch (n) {
        case 1:
            mFormat = GL_RED;
            mInternalFormat = GL_RED;
            break;
        case 2:
            mFormat = GL_RG;
            mInternalFormat = GL_RG;
            break;
        case 3:
            mFormat = GL_RGB;
            mInternalFormat = GL_RGB;
            break;
        case 4:
            mFormat = GL_RGBA;
            mInternalFormat = GL_RGBA;
            break;
        default:
            throw std::runtime_error("Bitmap Format " + std::to_string(mFormat) + " not recognized");
    }

    mWidth = w;
    mHeight = h;
    mType = GL_UNSIGNED_BYTE;
}

Bitmap::Bitmap(const Bitmap &rhs)
        : mWidth(rhs.mWidth), mHeight(rhs.mHeight), mFormat(rhs.mFormat),
          mInternalFormat(rhs.mInternalFormat), mType(rhs.mType)
{
    auto data = new uint8_t[rhs.GetBytes()];
    memcpy(data, rhs.mData.get(), rhs.GetBytes());
    mData.reset(data);
}

Bitmap &Bitmap::operator=(const Bitmap &rhs) {
    if(this != &rhs) {
        mWidth = rhs.mWidth;
        mHeight = rhs.mHeight;
        mFormat = rhs.mFormat;
        mType = rhs.mType;
        auto data = new uint8_t[rhs.GetBytes()];
        memcpy(data, rhs.mData.get(), rhs.GetBytes());
        mData.reset(data);
    }
    return *this;
}

void Bitmap::SetData(uint8_t *data, uint32_t width, uint32_t height, uint32_t format, uint32_t type) {
    mData.reset(data);
    mWidth = width;
    mHeight = height;
    mFormat = format;
    mInternalFormat = format;
    mType = type;
}

const uint8_t *Bitmap::GetData() const {
    return mData.get();
}

uint32_t Bitmap::GetWidth() const {
    return mWidth;
}

uint32_t Bitmap::GetHeight() const {
    return mHeight;
}

uint32_t Bitmap::GetFormat() const {
    return mFormat;
}

uint32_t Bitmap::GetInternalFormat() const {
    return mInternalFormat;
}

uint32_t Bitmap::GetType() const {
    return mType;
}

uint32_t Bitmap::GetComponents() const {
    switch (mFormat) {
        case GL_RED:
            return 1;
        case GL_RG:
            return 2;
        case GL_RGB:
            return 3;
        case GL_RGBA:
            return 4;
        case GL_DEPTH_COMPONENT:
            return 1;
        case GL_DEPTH_STENCIL:
            return 2;
        default:
            throw std::runtime_error("Bitmap Format " + std::to_string(mFormat) + " not recognized");
    }
}

size_t Bitmap::GetBytes() const {
    return mWidth*mHeight*GetComponents()*GetTypeBytes();
}

size_t Bitmap::GetTypeBytes() const {
    switch (mType) {
        case GL_UNSIGNED_BYTE:
            return sizeof(uint8_t);
        case GL_FLOAT:
            return sizeof(float);
        case GL_INT:
            return sizeof(int32_t);
        case GL_UNSIGNED_INT:
            return sizeof(uint32_t);
        default:
            throw std::runtime_error("Bitmap Type " + std::to_string(mType) + " not recognized");
    }
}
