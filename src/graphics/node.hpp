//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_NODE_HPP
#define ENGINE3D_SRC_NODE_HPP

#include <math/transform.hpp>
#include <list>
#include <functional>
#include <components/component.hpp>
#include <controllers/controller.hpp>
#include "mesh.hpp"
#include "meshbatch.hpp"
#include "bone.hpp"

class Scene;

class Node : public std::enable_shared_from_this<Node> {
public:
    static std::shared_ptr<Node> Load(const filesystem::path& file, Scene* scene);

    virtual ~Node() = default;

    void Update(float dt);
    void Render(MeshBatch* batch);

    void AddNode(const std::shared_ptr<Node> &node);
    bool RemoveNode(const Node *node);
    bool RemoveNode(const std::string &name);
    Node* FindNode(const std::string &name);

    template<class Pred>
    [[nodiscard]] std::list<Node*> FilterNode(Pred &&pred) {
        std::list<Node*> list;
        if(pred(this)) {
            list.push_back(this);
        }

        for(const auto& child : mChildren) {
            list.splice(list.end(), child->FilterNode<Pred>(pred));
        }

        return list;
    }

    void Apply(const std::function<void(Node*)> &f) {
        f(this);
        for(auto& child : mChildren) {
            child->Apply(f);
        }
    }

    [[nodiscard]] const std::string &GetName() const;
    void SetName(const std::string &name);

    [[nodiscard]] const std::vector<std::shared_ptr<Mesh>> &GetMeshes() const;
    [[nodiscard]] const std::shared_ptr<Mesh> &GetMesh(uint32_t index) const;
    void AddMesh(const std::shared_ptr<Mesh> &mesh);

    [[nodiscard]] const std::weak_ptr<Node> &GetParent() const;
    [[nodiscard]] std::weak_ptr<Node> GetRoot();

    [[nodiscard]] const std::list<std::shared_ptr<Node>> &GetChildren() const;
    void SetChildren(const std::list<std::shared_ptr<Node>> &children);

    [[nodiscard]] const std::shared_ptr<Bone> &GetBone() const;
    void SetBone(const std::shared_ptr<Bone> &bone);

    [[nodiscard]] const Transform &GetLocalTransform() const;
    void SetLocalTransform(const Transform &localTransform);

    [[nodiscard]] const glm::mat4 &GetWorldMatrix() const;

    [[nodiscard]] const glm::vec3 &GetPosition() const;
    void SetPosition(const glm::vec3 &position);

    [[nodiscard]] const glm::quat &GetRotation() const;
    void SetRotation(const glm::quat &rotation);

    [[nodiscard]] const glm::vec3 &GetScale() const;
    void SetScale(const glm::vec3 &scale);

    template<class T>
    void SetController(const std::shared_ptr<T> &controller) {
        controller->SetTarget(shared_from_this());
        mController = std::dynamic_pointer_cast<Controller>(controller);
    }

    template<class T>
    std::shared_ptr<T> GetController() const {
        return std::dynamic_pointer_cast<T>(mController);
    }

    [[nodiscard]] const std::list<std::shared_ptr<Component>> &GetComponents() const;

    template<class T>
    void AddComponent(const std::shared_ptr<T> &component) {
        component->SetTarget(shared_from_this());
        mComponents.push_back(std::dynamic_pointer_cast<Component>(component));
    }

    template<class T>
    std::shared_ptr<T> GetComponent() const {
        for(auto &component : mComponents) {
            if(dynamic_cast<T*>(component.get())) {
                return std::dynamic_pointer_cast<T>(component);
            }
        }
        return nullptr;
    }

protected:
    void ParentWorldMatrixChanged();

private:
    std::string mName;
    std::weak_ptr<Scene> pScene;
    std::vector< std::shared_ptr<Mesh> > mMeshes;
    std::weak_ptr<Node> pParent;
    std::list< std::shared_ptr<Node> > mChildren;
    std::shared_ptr<Bone> mBone;
    Transform mLocalTransform;
    mutable glm::mat4 mWorldMatrix{1.0f};
    mutable bool mNeedUpdateWorldMatrix{true};
    std::shared_ptr<Controller> mController;
    std::list< std::shared_ptr<Component> > mComponents;
};

#endif //ENGINE3D_SRC_NODE_HPP
