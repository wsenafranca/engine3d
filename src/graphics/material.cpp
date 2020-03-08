//
// Created by Washington on 23/02/2020.
//

#include "texture.hpp"
#include "material.hpp"

Texture *MaterialBuilder::GetDiffuseTexture() const {
    return mDiffuseTexture.get();
}

MaterialBuilder& MaterialBuilder::SetDiffuseTexture(Texture *diffuseTexture) {
    mDiffuseTexture.reset(diffuseTexture);
    mTextureBits |= MATERIAL_TEXTURE_DIFFUSE_BITS;
    return *this;
}

Texture *MaterialBuilder::GetSpecularTexture() const {
    return mSpecularTexture.get();
}

MaterialBuilder& MaterialBuilder::SetSpecularTexture(Texture *specularTexture) {
    mSpecularTexture.reset(specularTexture);
    mTextureBits |= MATERIAL_TEXTURE_SPECULAR_BITS;
    return *this;
}

Texture *MaterialBuilder::GetOcclusionTexture() const {
    return mOcclusionTexture.get();
}

MaterialBuilder& MaterialBuilder::SetOcclusionTexture(Texture *occlusionTexture) {
    mOcclusionTexture.reset(occlusionTexture);
    mTextureBits |= MATERIAL_TEXTURE_OCCLUSION_BITS;
    return *this;
}

Texture *MaterialBuilder::GetNormalTexture() const {
    return mNormalTexture.get();
}

MaterialBuilder& MaterialBuilder::SetNormalTexture(Texture *normalTexture) {
    mNormalTexture.reset(normalTexture);
    mTextureBits |= MATERIAL_TEXTURE_NORMAL_BITS;
    return *this;
}

Texture *MaterialBuilder::GetHeightTexture() const {
    return mHeightTexture.get();
}

MaterialBuilder& MaterialBuilder::SetHeightTexture(Texture *heightTexture) {
    mHeightTexture.reset(heightTexture);
    mTextureBits |= MATERIAL_TEXTURE_HEIGHT_BITS;
    return *this;
}

const glm::vec3 &MaterialBuilder::GetDiffuseColor() const {
    return mDiffuseColor;
}

MaterialBuilder& MaterialBuilder::SetDiffuseColor(const glm::vec3 &diffuseColor) {
    mDiffuseColor = diffuseColor;
    return *this;
}

const glm::vec3 &MaterialBuilder::GetSpecularColor() const {
    return mSpecularColor;
}

MaterialBuilder& MaterialBuilder::SetSpecularColor(const glm::vec3 &specularColor) {
    mSpecularColor = specularColor;
    return *this;
}

const glm::vec3 &MaterialBuilder::GetAmbientColor() const {
    return mAmbientColor;
}

MaterialBuilder& MaterialBuilder::SetAmbientColor(const glm::vec3 &ambientColor) {
    mAmbientColor = ambientColor;
    return *this;
}

float MaterialBuilder::GetShininess() const {
    return mShininess;
}

MaterialBuilder& MaterialBuilder::SetShininess(float shininess) {
    mShininess = shininess;
    return *this;
}

float MaterialBuilder::GetOpacity() const {
    return mOpacity;
}

MaterialBuilder& MaterialBuilder::SetOpacity(float opacity) {
    mOpacity = opacity;
    return *this;
}

MaterialBlendMode MaterialBuilder::GetBlendMode() const {
    return mBlendMode;
}

MaterialBuilder& MaterialBuilder::SetBlendMode(MaterialBlendMode blendMode) {
    mBlendMode = blendMode;
    return *this;
}

bool MaterialBuilder::IsBackfaceCulling() const {
    return mBackfaceCulling;
}

MaterialBuilder& MaterialBuilder::SetBackfaceCulling(bool backfaceCulling) {
    mBackfaceCulling = backfaceCulling;
    return *this;
}

std::shared_ptr<Material> MaterialBuilder::Build() {
    auto material = std::make_shared<Material>();
    material->diffuseTexture = mDiffuseTexture;
    material->specularTexture = mSpecularTexture;
    material->occlusionTexture = mOcclusionTexture;
    material->normalTexture = mNormalTexture;
    material->heightTexture = mHeightTexture;
    material->diffuseColor = mDiffuseColor;
    material->specularColor = mSpecularColor;
    material->ambientColor = mAmbientColor;
    material->shininess = mShininess;
    material->opacity = mOpacity;
    material->blendMode = mBlendMode;
    material->backfaceCulling = mBackfaceCulling;
    material->textureBits = mTextureBits;
    return material;
}
