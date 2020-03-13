//
// Created by Washington on 13/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_DYNAMICTREE_HPP
#define ENGINE3D_SRC_PHYSICS_DYNAMICTREE_HPP

#include "bounds.hpp"
#include "ray.hpp"
#include <stack>
#include <functional>

struct DynamicTreeNode {
    Bounds bounds;
    union {
        int32_t parent;
        int32_t next;
    };

    int32_t child1;
    int32_t child2;
    int32_t height;
    bool moved;

    [[nodiscard]] inline bool IsLeaf() const {return child1 == -1;}
};

class DynamicTree {
public:
    DynamicTree();
    ~DynamicTree();

    int32_t CreateProxy(const Bounds &bounds);
    void DestroyProxy(int32_t proxyId);
    bool MoveProxy(int32_t proxyId, const Bounds &bounds, const glm::vec3 &displacement);
    [[nodiscard]] bool WasMoved(int32_t proxyId) const;
    void ClearMoved(int32_t proxyId);
    [[nodiscard]] const Bounds &GetFatBounds(int32_t proxyId) const;
    void Query(const std::function<bool(int32_t)> &callback, const Bounds &bounds) const;
    void RayCast(const std::function<float(const Ray&, uint32_t)> &callback, const Ray &ray) const;
    [[nodiscard]] int32_t GetHeight() const;
    [[nodiscard]] int32_t GetMaxBalance() const;
    [[nodiscard]] float GetAreaRatio() const;
    void RebuildBottomUp();
    void ShiftOrigin(const glm::vec3 &newOrigin);

private:
    int32_t AllocateNode();
    void FreeNode(int32_t nodeId);

    void InsertLeaf(int32_t node);
    void RemoveLeaf(int32_t node);

    int32_t Balance(int32_t index);

    [[nodiscard]] int32_t ComputeHeight() const;
    [[nodiscard]] int32_t ComputeHeight(int32_t nodeId) const;

    void ValidateStructure(int32_t index) const;
    void ValidateMetrics(int32_t index) const;

    int32_t mRoot{-1};
    DynamicTreeNode *mNodes{nullptr};
    uint32_t mNodeCount{0};
    uint32_t mNodeCapacity{16};

    int32_t mFreeList{0};
    uint32_t mPath{0};
    int32_t mInsertionCount{0};
};

#endif //ENGINE3D_SRC_PHYSICS_DYNAMICTREE_HPP
