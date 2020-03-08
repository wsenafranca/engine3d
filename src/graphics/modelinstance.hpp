//
// Created by Washington on 06/03/2020.
//

#ifndef ENGINE3D_SRC_GRAPHICS_MODELINSTANCE_HPP
#define ENGINE3D_SRC_GRAPHICS_MODELINSTANCE_HPP

#include "model.hpp"

class ModelInstance {
public:
    explicit ModelInstance(const std::shared_ptr<Model> &model);
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

    [[nodiscard]] const std::shared_ptr<Model> &GetModel() const;

    const std::unordered_map<std::string, Transform> &GetNodeTransformMap() const;
    void SetNodeTransformMap(const std::unordered_map<std::string, Transform> &nodeTransformMap);

    const Transform &GetNodeTransform(const std::string &name) const;
    void SetNodeTransform(const std::string &name, const Transform &transform);

protected:
    void RenderNode(MeshBatch* batch, const std::shared_ptr<Node> &node, const glm::mat4 &parentMatrix, const glm::mat4 &worldMatrix);

private:
    std::shared_ptr<Model> pModel;
    Transform mTransform;
    std::vector<glm::mat4> mBoneMatrices;
    std::unordered_map<std::string, Transform> mNodeTransformMap;
};


#endif //ENGINE3D_SRC_GRAPHICS_MODELINSTANCE_HPP
