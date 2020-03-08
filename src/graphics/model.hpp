//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_MODEL_HPP
#define ENGINE3D_SRC_MODEL_HPP

#include "mesh.hpp"
#include "material.hpp"
#include "light.hpp"
#include "node.hpp"

class ModelInstance;

class Model : public std::enable_shared_from_this<Model> {
public:
    Model();
    explicit Model(const filesystem::path& file);

    void SetRootNode(const std::shared_ptr<Node> &node);
    [[nodiscard]] const std::shared_ptr<Node> &GetRootNode() const;
    [[nodiscard]] std::shared_ptr<Node> FindNode(const std::string& name) const;

    void AddTexture(const std::shared_ptr<Texture> &texture);
    [[nodiscard]] const std::shared_ptr<Texture> &GetTexture(uint32_t index) const;

    void AddMesh(const std::shared_ptr<Mesh> &mesh);
    [[nodiscard]] const std::shared_ptr<Mesh> &GetMesh(uint32_t index) const;

    void AddLight(const std::shared_ptr<Light> &light);
    [[nodiscard]] const std::shared_ptr<Light> &GetLight(uint32_t index) const;

    std::shared_ptr<Bone> CreateBone(const std::string& name);
    [[nodiscard]] const std::shared_ptr<Bone> &GetBone(uint32_t index) const;

    [[nodiscard]] const std::unordered_map<std::string, Transform> &GetNodeTransformMap() const;
    [[nodiscard]] const Transform &GetNodeTransform(const std::string& nodeName) const;
    void SetNodeTransform(const std::string &nodeName, const Transform &transform);

    std::shared_ptr<ModelInstance> CreateInstance();
    void DestroyInstance(ModelInstance* instance);

    friend class ModelInstance;

protected:
    static std::shared_ptr<Node> FindNode(const std::string& name, const std::shared_ptr<Node> &node);

private:
    std::shared_ptr<Model> mSharedPtr;
    std::shared_ptr<Node> mRootNode;
    std::vector< std::shared_ptr<Texture> > mTextures;
    std::vector< std::shared_ptr<Mesh> > mMeshes;
    std::vector< std::shared_ptr<Light> > mLights;
    std::vector< std::shared_ptr<Bone> > mBones;
    std::unordered_map<std::string, uint32_t> mBoneIndexMap;
    std::list< std::shared_ptr<ModelInstance> > mInstances;
    std::unordered_map< std::string, Transform> mNodeTransformMap;
};

#endif //ENGINE3D_SRC_MODEL_HPP
