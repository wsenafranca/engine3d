//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_BITMAP_HPP
#define ENGINE3D_SRC_BITMAP_HPP

#include <cstdint>
#include <filesystem/path.h>
#include <glm/glm.hpp>
#include <memory>

class Bitmap {
public:
    Bitmap();
    Bitmap(uint8_t *data, uint32_t width, uint32_t height, uint32_t format, uint32_t type);
    explicit Bitmap(const filesystem::path& file);
    Bitmap(const Bitmap& rhs);
    Bitmap& operator=(const Bitmap& rhs);

    [[nodiscard]] const uint8_t* GetData() const;
    void SetData(uint8_t *data, uint32_t width, uint32_t height, uint32_t format, uint32_t type);

    [[nodiscard]] uint32_t GetWidth() const;

    [[nodiscard]] uint32_t GetHeight() const;

    [[nodiscard]] uint32_t GetFormat() const;

    [[nodiscard]] uint32_t GetInternalFormat() const;

    [[nodiscard]] uint32_t GetType() const;

    [[nodiscard]] uint32_t GetComponents() const;

    [[nodiscard]] size_t GetBytes() const;

    [[nodiscard]] size_t GetTypeBytes() const;
private:
    std::unique_ptr<uint8_t> mData;
    uint32_t mWidth, mHeight, mFormat, mInternalFormat, mType;
};


#endif //ENGINE3D_SRC_BITMAP_HPP
