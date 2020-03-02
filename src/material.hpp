//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_MATERIAL_HPP
#define ENGINE3D_SRC_MATERIAL_HPP

#include <glm/glm.hpp>
#include "texture.hpp"

enum MaterialBlendMode {
    MATERIAL_BLEND_MODE_DEFAULT = 0,
    MATERIAL_BLEND_MODE_MASK = 1,
    MATERIAL_BLEND_MODE_ADDITIVE = 2
};

enum MaterialTextureBits {
    MATERIAL_TEXTURE_DIFFUSE_BITS = 1 << 0,
    MATERIAL_TEXTURE_SPECULAR_BITS = 1 << 1,
    MATERIAL_TEXTURE_OCCLUSION_BITS = 1 << 2,
    MATERIAL_TEXTURE_NORMAL_BITS = 1 << 3,
    MATERIAL_TEXTURE_HEIGHT_BITS = 1 << 4
};

enum MaterialTextureSlot {
    MATERIAL_TEXTURE_DIFFUSE_SLOT = 0,
    MATERIAL_TEXTURE_SPECULAR_SLOT = 1,
    MATERIAL_TEXTURE_OCCLUSION_SLOT= 2,
    MATERIAL_TEXTURE_NORMAL_SLOT= 3,
    MATERIAL_TEXTURE_HEIGHT_SLOT= 4,
    MATERIAL_TEXTURE_FREE0_SLOT = 5
};

struct Material {
    std::shared_ptr<Texture> diffuseTexture{nullptr};
    std::shared_ptr<Texture> specularTexture{nullptr};
    std::shared_ptr<Texture> occlusionTexture{nullptr};
    std::shared_ptr<Texture> normalTexture{nullptr};
    std::shared_ptr<Texture> heightTexture{nullptr};
    glm::vec3 diffuseColor{1.0f, 1.0f, 1.0f};
    glm::vec3 specularColor{1.0f, 1.0f, 1.0f};
    glm::vec3 ambientColor{0.1f, 0.1f, 0.1f};
    float shininess{32.0f};
    float opacity{1.0f};
    MaterialBlendMode blendMode{MATERIAL_BLEND_MODE_DEFAULT};
    bool backfaceCulling{true};
    uint32_t textureBits{0};
};

class MaterialBuilder {
public:
    [[nodiscard]] Texture *GetDiffuseTexture() const;
    MaterialBuilder& SetDiffuseTexture(Texture *diffuseTexture);
    [[nodiscard]] Texture *GetSpecularTexture() const;
    MaterialBuilder& SetSpecularTexture(Texture *specularTexture);
    [[nodiscard]] Texture *GetOcclusionTexture() const;
    MaterialBuilder& SetOcclusionTexture(Texture *occlusionTexture);
    [[nodiscard]] Texture *GetNormalTexture() const;
    MaterialBuilder& SetNormalTexture(Texture *normalTexture);
    [[nodiscard]] Texture *GetHeightTexture() const;
    MaterialBuilder& SetHeightTexture(Texture *heightTexture);
    [[nodiscard]] const glm::vec3 &GetDiffuseColor() const;
    MaterialBuilder& SetDiffuseColor(const glm::vec3 &diffuseColor);
    [[nodiscard]] const glm::vec3 &GetSpecularColor() const;
    MaterialBuilder& SetSpecularColor(const glm::vec3 &specularColor);
    [[nodiscard]] const glm::vec3 &GetAmbientColor() const;
    MaterialBuilder& SetAmbientColor(const glm::vec3 &ambientColor);
    [[nodiscard]] float GetShininess() const;
    MaterialBuilder& SetShininess(float shininess);
    [[nodiscard]] float GetOpacity() const;
    MaterialBuilder& SetOpacity(float opacity);
    [[nodiscard]] MaterialBlendMode GetBlendMode() const;
    MaterialBuilder& SetBlendMode(MaterialBlendMode alphaMode);
    [[nodiscard]] bool IsBackfaceCulling() const;
    MaterialBuilder& SetBackfaceCulling(bool backfaceCulling);

    Material Build();
private:
    std::shared_ptr<Texture> mDiffuseTexture{nullptr};
    std::shared_ptr<Texture> mSpecularTexture{nullptr};
    std::shared_ptr<Texture> mOcclusionTexture{nullptr};
    std::shared_ptr<Texture> mNormalTexture{nullptr};
    std::shared_ptr<Texture> mHeightTexture{nullptr};
    glm::vec3 mDiffuseColor{1.0f};
    glm::vec3 mSpecularColor{0.5f};
    glm::vec3 mAmbientColor{0.1f};
    float mShininess{32.0f};
    float mOpacity{1.0f};
    MaterialBlendMode mBlendMode{MATERIAL_BLEND_MODE_DEFAULT};
    bool mBackfaceCulling{true};
    uint32_t mTextureBits{0};
};

#endif //ENGINE3D_SRC_MATERIAL_HPP
