//
// Created by Washington on 10/03/2020.
//

#include "node.hpp"
#include <scenes/scene.hpp>
#include "../internal/importer.hpp"

std::shared_ptr<Node> Node::Load(const filesystem::path& file, Scene* scene) {
    return Importer().LoadNode(file, scene);
}

void Node::Update(float dt) {
    if(mController) {
        mController->Update(dt);
    }

    for(auto& component : mComponents) {
        component->Update(dt);
    }

    for(auto& child : mChildren) {
        child->Update(dt);
    }
}

void Node::Render(MeshBatch *batch) {
    for(auto& mesh : mMeshes) {
        std::vector<glm::mat4> boneMatrices{};
        if(mesh->skeleton) {
            auto root = mesh->skeleton->root.lock();
            auto inverseRootMatrix = glm::inverse(root->GetWorldMatrix());
            for(auto &ref : mesh->skeleton->joints) {
                auto joint = ref.lock();
                if(joint && joint->GetBone()) {
                    boneMatrices.push_back(inverseRootMatrix*joint->GetWorldMatrix()*joint->GetBone()->offsetMatrix);
                }
            }
        }
        batch->Render(mesh.get(), GetWorldMatrix(), boneMatrices);
    }
    for(auto& child : mChildren) {
        child->Render(batch);
    }
}

void Node::AddNode(const std::shared_ptr<Node> &node) {
    mChildren.push_back(node);
    node->pParent = shared_from_this();
}

bool Node::RemoveNode(const Node *node) {
    if(!node) return false;
    return RemoveNode(node->mName);
}

bool Node::RemoveNode(const std::string &name) {
    for(auto it = mChildren.begin(); it != mChildren.end(); ++it) {
        if((*it)->mName == name) {
            (*it)->pParent.reset();
            mChildren.erase(it);
            return true;
        }
    }

    for(auto& child : mChildren) {
        if(child->RemoveNode(name)) {
            return true;
        }
    }

    return false;
}

Node* Node::FindNode(const std::string &name) {
    if(mName == name) {
        return this;
    }

    for(auto& child : mChildren) {
        auto ret = child->FindNode(name);
        if(ret) {
            return ret;
        }
    }

    return nullptr;
}

const std::string &Node::GetName() const {
    return mName;
}

void Node::SetName(const std::string &name) {
    mName = name;
}

const std::vector<std::shared_ptr<Mesh>> &Node::GetMeshes() const {
    return mMeshes;
}

const std::shared_ptr<Mesh> &Node::GetMesh(uint32_t index) const {
    return mMeshes[index];
}

void Node::AddMesh(const std::shared_ptr<Mesh> &mesh) {
    mMeshes.push_back(mesh);
}

const std::weak_ptr<Node> &Node::GetParent() const {
    return pParent;
}

std::weak_ptr<Node> Node::GetRoot() {
    auto lock = pParent.lock();
    return lock ? lock->GetRoot() : std::weak_ptr<Node>(shared_from_this());
}

const std::list<std::shared_ptr<Node>> &Node::GetChildren() const {
    return mChildren;
}

void Node::SetChildren(const std::list<std::shared_ptr<Node>> &children) {
    mChildren = children;
}

const std::shared_ptr<Bone> &Node::GetBone() const {
    return mBone;
}

void Node::SetBone(const std::shared_ptr<Bone> &bone) {
    mBone = bone;
}

const Transform &Node::GetLocalTransform() const {
    return mLocalTransform;
}

void Node::SetLocalTransform(const Transform &localTransform) {
    if(mLocalTransform != localTransform) {
        mLocalTransform = localTransform;
        ParentWorldMatrixChanged();
    }
}

const glm::mat4 &Node::GetWorldMatrix() const {
    if(mNeedUpdateWorldMatrix) {
        mNeedUpdateWorldMatrix = true;
        auto lock = pParent.lock();
        if(lock) {
            mWorldMatrix = lock->GetWorldMatrix();
        } else {
            mWorldMatrix = glm::mat4(1.0f);
        }
        mWorldMatrix = mWorldMatrix*mLocalTransform.ToMatrix();
    }
    return mWorldMatrix;
}

void Node::ParentWorldMatrixChanged() {
    mNeedUpdateWorldMatrix = true;
    for(auto& child : mChildren) {
        child->ParentWorldMatrixChanged();
    }
}

const glm::vec3 &Node::GetPosition() const {
    return mLocalTransform.position;
}

void Node::SetPosition(const glm::vec3 &position) {
    if(mLocalTransform.position != position) {
        mLocalTransform.position = position;
        ParentWorldMatrixChanged();
    }
}

const glm::quat &Node::GetRotation() const {
    return mLocalTransform.rotation;
}

void Node::SetRotation(const glm::quat &rotation) {
    if(mLocalTransform.rotation != rotation) {
        mLocalTransform.rotation = rotation;
        ParentWorldMatrixChanged();
    }
}

const glm::vec3 &Node::GetScale() const {
    return mLocalTransform.scale;
}

void Node::SetScale(const glm::vec3 &scale) {
    if(mLocalTransform.scale != scale) {
        mLocalTransform.scale = scale;
        ParentWorldMatrixChanged();
    }
}

const std::list<std::shared_ptr<Component>> &Node::GetComponents() const {
    return mComponents;
}
