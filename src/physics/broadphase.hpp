//
// Created by Washington on 13/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_BROADPHASE_HPP
#define ENGINE3D_SRC_PHYSICS_BROADPHASE_HPP

#include "dynamictree.hpp"

struct BroadPhasePair {
    int32_t proxyIdA;
    int32_t proxyIdB;
};

class BroadPhase {
public:
    BroadPhase();
    ~BroadPhase();

    int32_t CreateProxy(const Bounds &bounds);
    void DestroyProxy(int32_t proxyId);
    void MoveProxy(int32_t proxyId, const Bounds &bounds, const glm::vec3 &displacement);
    void TouchProxy(int32_t proxyId);

    const Bounds &GetFatBounds(int32_t proxyId) const;
    bool TestOverlap(int32_t proxyIdA, int32_t proxyIdB) const;
    uint32_t GetProxyCount() const;

    void UpdatePairs(const std::function<void(int32_t, int32_t)> &callback);
    void Query(const std::function<bool(int32_t)> &callback, const Bounds &bounds) const;
    void RayCast(const std::function<float(const Ray&, uint32_t)> &callback, const Ray &ray) const;

    uint32_t GetTreeHeight() const;
    uint32_t GetTreeBalance() const;
    float GetTreeQuality() const;
    void ShiftOrigin(const glm::vec3 &newOrigin);

private:
    void BufferMove(int32_t proxyId);
    void UnBufferMove(int32_t proxyId);
    bool QueryCallback(int32_t proxyId);

    DynamicTree mTree;
    uint32_t mProxyCount{0};
    int32_t *mMoveBuffer;
    uint32_t mMoveCapacity{16};
    uint32_t mMoveCount{0};

    BroadPhasePair *mPairBuffer;
    uint32_t mPairCapacity{16};
    uint32_t mPairCount{0};
    int32_t mQueryProxyId{-1};
};


#endif //ENGINE3D_SRC_PHYSICS_BROADPHASE_HPP
