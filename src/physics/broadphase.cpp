//
// Created by Washington on 13/03/2020.
//

#include "broadphase.hpp"

BroadPhase::BroadPhase() {
    mPairBuffer = (BroadPhasePair*)malloc(sizeof(BroadPhasePair)*mPairCapacity);
    mMoveBuffer = (int32_t *)malloc(sizeof(int32_t)*mPairCapacity);
}

BroadPhase::~BroadPhase() {
    free(mPairBuffer);
    free(mMoveBuffer);
}

int32_t BroadPhase::CreateProxy(const Bounds &bounds) {
    int32_t proxyId = mTree.CreateProxy(bounds);
    ++mProxyCount;
    BufferMove(proxyId);
    return proxyId;
}

void BroadPhase::DestroyProxy(int32_t proxyId) {
    UnBufferMove(proxyId);
    --mProxyCount;
    mTree.DestroyProxy(proxyId);
}

void BroadPhase::MoveProxy(int32_t proxyId, const Bounds &bounds, const glm::vec3 &displacement) {
    bool buffer = mTree.MoveProxy(proxyId, bounds, displacement);
    if (buffer) {
        BufferMove(proxyId);
    }
}

void BroadPhase::TouchProxy(int32_t proxyId) {
    BufferMove(proxyId);
}

const Bounds &BroadPhase::GetFatBounds(int32_t proxyId) const {
    return mTree.GetFatBounds(proxyId);
}

bool BroadPhase::TestOverlap(int32_t proxyIdA, int32_t proxyIdB) const {
    const Bounds& aabbA = mTree.GetFatBounds(proxyIdA);
    const Bounds& aabbB = mTree.GetFatBounds(proxyIdB);
    return Physics::Intersects(aabbA, aabbB);
}

uint32_t BroadPhase::GetProxyCount() const {
    return mProxyCount;
}

void BroadPhase::UpdatePairs(const std::function<void(int32_t, int32_t)> &callback) {
    // Reset pair buffer
    mPairCount = 0;
    auto cb = [this](int32_t proxyId) -> bool {
        return this->QueryCallback(proxyId);
    };

    // Perform tree queries for all moving proxies.
    for (uint32_t i = 0; i < mMoveCount; ++i) {
        mQueryProxyId = mMoveBuffer[i];
        if (mQueryProxyId == -1)  {
            continue;
        }

        // We have to query the tree with the fat AABB so that
        // we don't fail to create a pair that may touch later.
        const Bounds& fatAABB = mTree.GetFatBounds(mQueryProxyId);

        // Query tree, create pairs and add them pair buffer.
        mTree.Query(cb, fatAABB);
    }

    // Send pairs to caller
    for (uint32_t i = 0; i < mPairCount; ++i) {
        BroadPhasePair* primaryPair = mPairBuffer + i;
        callback(primaryPair->proxyIdA, primaryPair->proxyIdB);
    }

    // Clear move flags
    for (uint32_t i = 0; i < mMoveCount; ++i) {
        int32_t proxyId = mMoveBuffer[i];
        if (proxyId == -1) {
            continue;
        }

        mTree.ClearMoved(proxyId);
    }

    // Reset move buffer
    mMoveCount = 0;
}

void BroadPhase::Query(const std::function<bool(int32_t)> &callback, const Bounds &bounds) const {
    mTree.Query(callback, bounds);
}

void BroadPhase::RayCast(const std::function<float(const Ray &, uint32_t)> &callback, const Ray &ray) const {
    mTree.RayCast(callback, ray);
}

uint32_t BroadPhase::GetTreeHeight() const {
    return mTree.GetHeight();
}

uint32_t BroadPhase::GetTreeBalance() const {
    return mTree.GetMaxBalance();
}

float BroadPhase::GetTreeQuality() const {
    return mTree.GetAreaRatio();
}

void BroadPhase::ShiftOrigin(const glm::vec3 &newOrigin) {
    mTree.ShiftOrigin(newOrigin);
}

void BroadPhase::BufferMove(int32_t proxyId) {
    if (mMoveCount == mMoveCapacity) {
        int32_t* oldBuffer = mMoveBuffer;
        mMoveCapacity *= 2;
        mMoveBuffer = (int32_t*)malloc(mMoveCapacity * sizeof(int32_t));
        memcpy(mMoveBuffer, oldBuffer, mMoveCount * sizeof(int32_t));
        free(oldBuffer);
    }

    mMoveBuffer[mMoveCount] = proxyId;
    ++mMoveCount;
}

void BroadPhase::UnBufferMove(int32_t proxyId) {
    for (uint32_t i = 0; i < mMoveCount; ++i) {
        if (mMoveBuffer[i] == proxyId) {
            mMoveBuffer[i] = -1;
        }
    }
}

bool BroadPhase::QueryCallback(int32_t proxyId) {
    // A proxy cannot form a pair with itself.
    if (proxyId == mQueryProxyId) {
        return true;
    }

    const bool moved = mTree.WasMoved(proxyId);
    if (moved && proxyId > mQueryProxyId) {
        // Both proxies are moving. Avoid duplicate pairs.
        return true;
    }

    // Grow the pair buffer as needed.
    if (mPairCount == mPairCapacity) {
        BroadPhasePair* oldBuffer = mPairBuffer;
        mPairCapacity = mPairCapacity + (mPairCapacity >> 1u);
        mPairBuffer = (BroadPhasePair*)malloc(mPairCapacity * sizeof(BroadPhasePair));
        memcpy(mPairBuffer, oldBuffer, mPairCount * sizeof(BroadPhasePair));
        free(oldBuffer);
    }

    mPairBuffer[mPairCount].proxyIdA = (glm::min)(proxyId, mQueryProxyId);
    mPairBuffer[mPairCount].proxyIdB = (glm::max)(proxyId, mQueryProxyId);
    ++mPairCount;

    return true;
}
