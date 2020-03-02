//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_TEXTURE_HPP
#define ENGINE3D_SRC_TEXTURE_HPP

#include <cstdint>
#include "bitmap.hpp"

class Texture {
public:
    explicit Texture(uint32_t target);
    virtual ~Texture();

    void SetBitmap(const Bitmap& bitmap);
    void SetBitmap(uint32_t bitmapTarget, const Bitmap& bitmap);
    void GenerateMipmap();

    void Bind() const;
    void BindActive(int textureSlot) const;
    void Unbind() const;

    void SetParameter(uint32_t pname, int32_t param);
    void SetParameter(uint32_t pname, float param);

    [[nodiscard]] uint32_t GetTarget() const;

    [[nodiscard]] uint32_t GetWidth() const;

    [[nodiscard]] uint32_t GetHeight() const;

    [[nodiscard]] uint32_t GetFormat() const;

private:
    uint32_t glTextureId{0};
    uint32_t mTarget{3553}; // GL_TEXTURE_2D
    uint32_t mWidth{0};
    uint32_t mHeight{0};
    uint32_t mFormat{0};
};

#endif //ENGINE3D_SRC_TEXTURE_HPP
