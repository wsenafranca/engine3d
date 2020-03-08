//
// Created by Washington on 25/02/2020.
//

#include "model.hpp"
#include "../internal/importer.hpp"
#include "modelinstance.hpp"

Model::Model() : mRootNode(nullptr) {
    mSharedPtr = std::shared_ptr<Model>(this);
}

Model::Model(const filesystem::path &file) {
    mSharedPtr = std::shared_ptr<Model>(this);
    Importer::LoadModel(this, file);
}

void Model::SetRootNode(const std::shared_ptr<Node> &node) {
    mRootNode = node;
}

const std::shared_ptr<Node> &Model::GetRootNode() const {
    return mRootNode;
}

std::shared_ptr<Node> Model::FindNode(const std::string &name) const {
    return FindNode(name, mRootNode);
}

std::shared_ptr<Node> Model::FindNode(const std::string &name, const std::shared_ptr<Node> &node) {
    if(name == node->name) {
        return node;
    }

    for(auto& child : node->children) {
        auto ret = FindNode(name, child);
        if(ret) {
            return ret;
        }
    }

    return nullptr;
}

void Model::AddTexture(const std::shared_ptr<Texture> &texture) {
    mTextures.push_back(texture);
}

const std::shared_ptr<Texture> &Model::GetTexture(uint32_t index) const {
    return mTextures[index];
}

void Model::AddMesh(const std::shared_ptr<Mesh> &mesh) {
    mMeshes.push_back(mesh);
}

const std::shared_ptr<Mesh> &Model::GetMesh(uint32_t index) const {
    return mMeshes[index];
}

void Model::AddLight(const std::shared_ptr<Light> &light) {
    mLights.push_back(light);
}

const std::shared_ptr<Light> &Model::GetLight(uint32_t index) const {
    return mLights[index];
}

std::shared_ptr<Bone> Model::CreateBone(const std::string& name) {
    auto it = mBoneIndexMap.find(name);
    if(it != mBoneIndexMap.end()) {
        return mBones[it->second];
    }

    auto bone = std::make_shared<Bone>();
    bone->name = name;
    bone->index = mBones.size();
    mBoneIndexMap[bone->name] = bone->index;
    mBones.push_back(bone);

    return bone;
}

const std::shared_ptr<Bone> &Model::GetBone(uint32_t index) const {
    return mBones[index];
}

const std::unordered_map<std::string, Transform> &Model::GetNodeTransformMap() const {
    return mNodeTransformMap;
}

const Transform &Model::GetNodeTransform(const std::string &nodeName) const {
    return mNodeTransformMap.at(nodeName);
}

void Model::SetNodeTransform(const std::string &nodeName, const Transform &transform) {
    mNodeTransformMap[nodeName] = transform;
}

std::shared_ptr<ModelInstance> Model::CreateInstance() {
    auto instance = std::make_shared<ModelInstance>(shared_from_this());
    mInstances.push_back(instance);
    return instance;
}

void Model::DestroyInstance(ModelInstance *instance) {
    auto it = std::find_if(mInstances.begin(), mInstances.end(), [instance](const std::shared_ptr<ModelInstance>& e) {
        return e.get() == instance;
    });

    if(it != mInstances.end()) {
        mInstances.erase(it);
    }
}
