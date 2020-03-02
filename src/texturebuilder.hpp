//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_TEXTUREBUILDER_HPP
#define ENGINE3D_SRC_TEXTUREBUILDER_HPP

#include "texture.hpp"
#include <unordered_map>
#include <list>

class TextureBuilder {
public:
    [[nodiscard]] uint32_t GetTarget() const;
    TextureBuilder& SetTarget(uint32_t target);

    int32_t GetParameterInt(uint32_t pname) const;
    TextureBuilder& SetParameter(uint32_t pname, int32_t param);

    [[nodiscard]] float GetParameterFloat(uint32_t pname) const;
    [[nodiscard]] TextureBuilder& SetParameter(uint32_t pname, float param);

    TextureBuilder& AddBitmap(const Bitmap& bitmap);
    TextureBuilder& AddBitmap(uint32_t target, const Bitmap& bitmap);
    [[nodiscard]] const Bitmap& GetBitmap(uint32_t index) const;

    TextureBuilder& SetTexture2D(const filesystem::path& file);
    TextureBuilder& SetSkyBox(const filesystem::path& directory, const std::string &fileExtension);

    bool IsGenerateMipmap() const;
    TextureBuilder& SetGenerateMipmap(bool generateMipmap);

    Texture* Build();
private:
    uint32_t mTarget{0};
    std::unordered_map<uint32_t, int32_t> mParamsInt;
    std::unordered_map<uint32_t, int32_t> mParamsFloat;
    std::vector< std::pair<uint32_t, Bitmap> > mBitmaps;
    bool mGenerateMipmap{true};
};


#endif //ENGINE3D_SRC_TEXTUREBUILDER_HPP
