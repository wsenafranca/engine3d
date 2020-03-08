//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_MESHBUILDER_HPP
#define ENGINE3D_SRC_MESHBUILDER_HPP

#include "mesh.hpp"

class MeshBuilder {
public:
    MeshBuilder& SetPositions(const std::vector<float>& positions);
    [[nodiscard]] const std::vector<float> &GetPositions() const;

    MeshBuilder& SetNormals(const std::vector<float>& normals);
    [[nodiscard]] const std::vector<float> &GetNormals() const;

    MeshBuilder& SetTexCoords(const std::vector<float>& texCoords);
    [[nodiscard]] const std::vector<float> &GetTexCoords() const;

    MeshBuilder& SetJoints(const std::vector<int>& joints);
    [[nodiscard]] const std::vector<int> &GetJoints() const;

    MeshBuilder& SetWeights(const std::vector<float>& weights);
    [[nodiscard]] const std::vector<float> &GetWeights() const;

    MeshBuilder& SetTangents(const std::vector<float>& tangents);
    [[nodiscard]] const std::vector<float> &GetTangents() const;

    MeshBuilder& SetIndices(const std::vector<uint32_t>& indices);
    [[nodiscard]] const std::vector<uint32_t> &GetIndices() const;

    MeshBuilder& SetMaterial(const std::shared_ptr<Material>& material);
    [[nodiscard]] const std::shared_ptr<Material> &GetMaterial() const;

    MeshBuilder& SetFlipTextureCoord(bool flipX, bool flipY);
    [[nodiscard]] bool IsFlipTextureCoordX() const;
    [[nodiscard]] bool IsFlipTextureCoordY() const;

    MeshBuilder& SetRenderMode(uint32_t mode);
    [[nodiscard]] uint32_t GetRenderMode() const;

    MeshBuilder& SetQuad(float width, float height);
    MeshBuilder& SetBox(const glm::vec3& size);
    MeshBuilder& SetPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);

    std::shared_ptr<Mesh> Build();

    friend class Mesh;
private:
    std::vector<float> mPositions;
    std::vector<float> mNormals;
    std::vector<float> mTexCoords;
    std::vector<int> mJoints;
    std::vector<float> mWeights;
    std::vector<float> mTangents;
    std::vector<uint32_t> mIndices;
    std::shared_ptr<Material> mMaterial;
    uint32_t mAttributeBits{0};
    uint32_t mRenderMode{4};
    bool mFlipTextureCoordX{false};
    bool mFlipTextureCoordY{true};
};

#endif //ENGINE3D_SRC_MESHBUILDER_HPP
