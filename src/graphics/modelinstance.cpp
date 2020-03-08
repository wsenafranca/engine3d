//
// Created by Washington on 06/03/2020.
//

#include "modelinstance.hpp"

ModelInstance::ModelInstance(const std::shared_ptr<Model> &model) : pModel(model) {
    mBoneMatrices.resize(model->mBones.size());
    mNodeTransformMap = model->GetNodeTransformMap();
}

void ModelInstance::Render(MeshBatch *batch) {
    auto worldMatrix = mTransform.ToMatrix();
    RenderNode(batch, pModel->GetRootNode(), worldMatrix, glm::inverse(worldMatrix));

    // reset transformations
    //mNodeTransformMap = pModel->GetNodeTransformMap();
}

void ModelInstance::RenderNode(MeshBatch *batch, const std::shared_ptr<Node> &node, const glm::mat4 &parentMatrix, const glm::mat4 &inverseWorldMatrix) {
    auto nodeMatrix = parentMatrix*mNodeTransformMap[node->name].ToMatrix();
    auto& model = pModel;

    if(node->bone) {
        mBoneMatrices[node->bone->index] = inverseWorldMatrix*nodeMatrix*node->bone->offsetMatrix;
    }

    if(node->camera) {
        node->camera->parentMatrix = nodeMatrix;
    }

    for(auto &mesh : node->meshes) {
        batch->Render(mesh.get(), nodeMatrix, &mBoneMatrices);
    }

    for(auto &child : node->children) {
        RenderNode(batch, child, nodeMatrix, inverseWorldMatrix);
    }
}

void ModelInstance::Translate(const glm::vec3 &translation) {
    mTransform.position += translation;
}

void ModelInstance::Rotate(const glm::quat &rotation) {
    mTransform.rotation *= rotation;
}

void ModelInstance::Scale(const glm::vec3 &scale) {
    mTransform.scale *= scale;
}

const glm::vec3 &ModelInstance::GetPosition() const {
    return mTransform.position;
}

void ModelInstance::SetPosition(const glm::vec3 &position) {
    mTransform.position = position;
}

const glm::quat &ModelInstance::GetRotation() const {
    return mTransform.rotation;
}

void ModelInstance::SetRotation(const glm::quat &rotation) {
    mTransform.rotation = rotation;
}

const glm::vec3 &ModelInstance::GetScale() const {
    return mTransform.scale;
}

void ModelInstance::SetScale(const glm::vec3 &scale) {
    mTransform.scale = scale;
}

const std::shared_ptr<Model> &ModelInstance::GetModel() const {
    return pModel;
}

const std::unordered_map<std::string, Transform> &ModelInstance::GetNodeTransformMap() const {
    return mNodeTransformMap;
}

void ModelInstance::SetNodeTransformMap(const std::unordered_map<std::string, Transform> &nodeTransformMap) {
    mNodeTransformMap = nodeTransformMap;
}

const Transform &ModelInstance::GetNodeTransform(const std::string &name) const {
    return mNodeTransformMap.at(name);
}

void ModelInstance::SetNodeTransform(const std::string &name, const Transform &transform) {
    mNodeTransformMap[name] = transform;
}

