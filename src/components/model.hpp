//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_MODEL_HPP
#define ENGINE3D_SRC_MODEL_HPP

#include "../graphics/mesh.hpp"
#include "../graphics/material.hpp"
#include "light.hpp"
#include "node.hpp"
#include "animgraph.hpp"

class ModelInstance;

class Model {
public:
    Model();
    explicit Model(const filesystem::path& file);

    void SetRootNode(Node* node);
    [[nodiscard]] Node* GetRootNode() const;
    [[nodiscard]] Node* FindNode(const std::string& name) const;

    void AddTexture(Texture* texture);
    [[nodiscard]] Texture* GetTexture(uint32_t index) const;

    void AddMesh(Mesh* mesh);
    [[nodiscard]] Mesh* GetMesh(uint32_t index) const;

    void AddLight(Light* light);
    [[nodiscard]] Light* GetLight(uint32_t index) const;

    Bone* CreateBone(const std::string& name);
    [[nodiscard]] Bone* GetBone(uint32_t index) const;

    ModelInstance* CreateInstance();
    void DestroyInstance(ModelInstance* instance);

    friend class ModelInstance;

protected:
    static Node* FindNode(const std::string& name, Node* node);

private:
    std::unique_ptr<Node> mRootNode;
    std::vector< std::unique_ptr<Texture> > mTextures;
    std::vector< std::unique_ptr<Mesh> > mMeshes;
    std::vector< std::unique_ptr<Light> > mLights;
    std::vector< std::unique_ptr<Bone> > mBones;
    std::unordered_map<std::string, uint32_t> mBoneIndexMap;
    std::list< std::unique_ptr<ModelInstance> > mInstances;
};

class ModelInstance {
public:
    explicit ModelInstance(Model* model);
    void Update(float dt);
    void Render(MeshBatch* batch);

    void Translate(const glm::vec3& translation);
    void Rotate(const glm::quat& rotation);
    void Scale(const glm::vec3& scale);

    [[nodiscard]] const glm::vec3 &GetPosition() const;
    void SetPosition(const glm::vec3 &position);

    [[nodiscard]] const glm::quat &GetRotation() const;
    void SetRotation(const glm::quat &rotation);

    [[nodiscard]] const glm::vec3 &GetScale() const;
    void SetScale(const glm::vec3 &scale);

    [[nodiscard]] Model *GetModel() const;

    [[nodiscard]] AnimGraph *GetAnimGraph() const;
    void SetAnimGraph(AnimGraph *animGraph);

protected:
    void RenderNode(MeshBatch* batch, Node* node, const glm::mat4 &parentMatrix, const glm::mat4 &worldMatrix);

private:
    Model* pModel{nullptr};
    Transform mTransform;
    std::vector<glm::mat4> mBoneMatrices;
    std::unique_ptr<AnimGraph> mAnimGraph;
};

#endif //ENGINE3D_SRC_MODEL_HPP
