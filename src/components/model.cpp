//
// Created by Washington on 25/02/2020.
//

#include "model.hpp"
#include "../internal/importer.hpp"

Model::Model() : mRootNode(nullptr) {

}

Model::Model(const filesystem::path &file) {
    Importer::LoadModel(this, file);
}

void Model::SetRootNode(Node *node) {
    mRootNode.reset(node);
}

Node *Model::GetRootNode() const {
    return mRootNode.get();
}

Node *Model::FindNode(const std::string &name) const {
    return FindNode(name, mRootNode.get());
}

Node *Model::FindNode(const std::string &name, Node *node) {
    if(name == node->name) {
        return node;
    }

    for(auto child : node->children) {
        auto ret = FindNode(name, child);
        if(ret) {
            return ret;
        }
    }

    return nullptr;
}

void Model::AddTexture(Texture *texture) {
    mTextures.emplace_back(texture);
}

Texture *Model::GetTexture(uint32_t index) const {
    return mTextures[index].get();
}

void Model::AddMesh(Mesh *mesh) {
    mMeshes.emplace_back(mesh);
}

Mesh *Model::GetMesh(uint32_t index) const {
    return mMeshes[index].get();
}

void Model::AddLight(Light *light) {
    mLights.emplace_back(light);
}

Light *Model::GetLight(uint32_t index) const {
    return mLights[index].get();
}

Bone *Model::CreateBone(const std::string& name) {
    auto it = mBoneIndexMap.find(name);
    if(it != mBoneIndexMap.end()) {
        return mBones[it->second].get();
    }

    auto bone = new Bone();
    bone->name = name;
    bone->index = mBones.size();
    mBoneIndexMap[bone->name] = bone->index;
    mBones.emplace_back(bone);

    return bone;
}

Bone *Model::GetBone(uint32_t index) const {
    return mBones[index].get();
}

ModelInstance *Model::CreateInstance() {
    auto instance = new ModelInstance(this);
    mInstances.emplace_back(instance);
    return instance;
}

void Model::DestroyInstance(ModelInstance *instance) {
    auto it = std::find_if(mInstances.begin(), mInstances.end(), [instance](const std::unique_ptr<ModelInstance>& e) {
        return e.get() == instance;
    });

    if(it != mInstances.end()) {
        mInstances.erase(it);
    }
}

ModelInstance::ModelInstance(Model *model) : pModel(model) {
    mBoneMatrices.resize(model->mBones.size());
}

void ModelInstance::Update(float dt) {
    if(mAnimGraph) {
        mAnimGraph->Update(dt);
    }
}

void ModelInstance::Render(MeshBatch *batch) {
    auto worldMatrix = mTransform.ToMatrix();
    RenderNode(batch, pModel->GetRootNode(), worldMatrix, glm::inverse(worldMatrix));
}

void ModelInstance::RenderNode(MeshBatch *batch, Node *node, const glm::mat4 &parentMatrix, const glm::mat4 &inverseWorldMatrix) {
    auto nodeMatrix = parentMatrix*node->matrix;
    auto& model = pModel;

    if(mAnimGraph) {
        if(mAnimGraph->GetCurrentAnimation()->HasChannel(node->name)) {
            nodeMatrix = parentMatrix*mAnimGraph->GetCurrentAnimationTransform(node->name).ToMatrix();
        }
    }

    if(node->bone) {
        mBoneMatrices[node->bone->index] = inverseWorldMatrix*nodeMatrix*node->bone->offsetMatrix;
    }

    if(node->camera) {
        node->camera->parentMatrix = nodeMatrix;
    }

    for(auto mesh : node->meshes) {
        batch->Render(mesh, nodeMatrix, &mBoneMatrices);
    }

    for(auto child : node->children) {
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

Model *ModelInstance::GetModel() const {
    return pModel;
}

AnimGraph *ModelInstance::GetAnimGraph() const {
    return mAnimGraph.get();
}

void ModelInstance::SetAnimGraph(AnimGraph *animGraph) {
    mAnimGraph.reset(animGraph);
}
