//
// Created by Washington on 25/02/2020.
//

#include "texturebuilder.hpp"
#include "opengl.hpp"

uint32_t TextureBuilder::GetTarget() const {
    return mTarget;
}

TextureBuilder &TextureBuilder::SetTarget(uint32_t target) {
    mTarget = target;
    return *this;
}

int32_t TextureBuilder::GetParameterInt(uint32_t pname) const {
    return mParamsInt.at(pname);
}

TextureBuilder &TextureBuilder::SetParameter(uint32_t pname, int32_t param) {
    mParamsInt[pname] = param;
    return *this;
}

float TextureBuilder::GetParameterFloat(uint32_t pname) const {
    return mParamsFloat.at(pname);
}

TextureBuilder &TextureBuilder::SetParameter(uint32_t pname, float param) {
    mParamsFloat[pname] = param;
    return *this;
}

TextureBuilder &TextureBuilder::AddBitmap(const Bitmap &bitmap) {
    return AddBitmap(mTarget, bitmap);
}

TextureBuilder &TextureBuilder::AddBitmap(uint32_t target, const Bitmap &bitmap) {
    mBitmaps.emplace_back(target, bitmap);
    return *this;
}

const Bitmap &TextureBuilder::GetBitmap(uint32_t index) const {
    return mBitmaps[index].second;
}

TextureBuilder &TextureBuilder::SetTexture2D(const filesystem::path &file) {
    mTarget = GL_TEXTURE_2D;
    return AddBitmap(Bitmap(file));
}

TextureBuilder &TextureBuilder::SetSkyBox(const filesystem::path &directory, const std::string &fileExtension) {
    mTarget = GL_TEXTURE_CUBE_MAP;
    AddBitmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X, Bitmap(directory/("right." + fileExtension)));
    AddBitmap(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Bitmap(directory/("left." + fileExtension)));
    AddBitmap(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Bitmap(directory/("top." + fileExtension)));
    AddBitmap(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Bitmap(directory/("bottom." + fileExtension)));
    AddBitmap(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Bitmap(directory/("front." + fileExtension)));
    AddBitmap(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Bitmap(directory/("back." + fileExtension)));
    return *this;
}

bool TextureBuilder::IsGenerateMipmap() const {
    return mGenerateMipmap;
}

TextureBuilder &TextureBuilder::SetGenerateMipmap(bool generateMipmap) {
    mGenerateMipmap = generateMipmap;
    return *this;
}

Texture *TextureBuilder::Build() {
    auto texture = new Texture(mTarget);
    texture->Bind();
    for(const auto& p : mBitmaps) {
        texture->SetBitmap(p.first, p.second);
    }
    for(const auto& p : mParamsInt) {
        texture->SetParameter(p.first, p.second);
    }
    for(const auto& p : mParamsFloat) {
        texture->SetParameter(p.first, p.second);
    }
    if(mGenerateMipmap) {
        texture->GenerateMipmap();
    }
    texture->Unbind();
    return texture;
}
