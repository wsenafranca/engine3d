//
// Created by Washington on 13/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_FIXTURE_HPP
#define ENGINE3D_SRC_PHYSICS_FIXTURE_HPP

#include <math/transform.hpp>
#include "bounds.hpp"
#include "ray.hpp"
#include "raycast.hpp"

class Fixture;
class Body;
class World;
class BroadPhase;

struct CollisionFilter {
    uint16_t categoryBits;
    uint16_t maskBits;
    uint16_t groupIndex;
};

struct FixtureDef {
    float friction;
    float restitution;
    float density;
    bool isSensor;
    CollisionFilter filter;
};

struct FixtureProxy {
    Bounds bounds;
    Fixture *fixture;
    int32_t childIndex;
    int32_t proxyId;
};

class Fixture {
public:
    Fixture(Body *body, const FixtureDef *def);
    ~Fixture();

    [[nodiscard]] float GetDensity() const;
    void SetDensity(float density);

    [[nodiscard]] Fixture *GetNext() const;

    [[nodiscard]] Body *GetBody() const;

    [[nodiscard]] float GetFriction() const;
    void SetFriction(float friction);

    [[nodiscard]] float GetRestitution() const;
    void SetRestitution(float restitution);

    [[nodiscard]] FixtureProxy *GetProxies() const;
    void SetProxies(FixtureProxy *proxies);

    [[nodiscard]] uint32_t GetProxyCount() const;
    void SetProxyCount(uint32_t proxyCount);

    [[nodiscard]] const CollisionFilter &GetFilterData() const;
    void SetFilterData(const CollisionFilter &filterData);

    [[nodiscard]] bool IsSensor() const;
    void SetIsSensor(bool isSensor);

    [[nodiscard]] bool TestPoint(const glm::vec3 &p) const;
    [[nodiscard]] bool RayCast(RayCastHit &hit, const Ray &ray) const;

    [[nodiscard]] const Bounds &GetBounds() const;

    friend class Fixture;
    friend class Body;
    friend class World;
    friend class BroadPhase;

protected:
    void CreateProxies(BroadPhase *broadPhase, const Transform &xf);
    void DestroyProxies(BroadPhase *broadPhase);
    void Synchronize(BroadPhase *broadPhase, const Transform &xf1, const Transform &xf2);

private:
    float mDensity{0.0f};
    Fixture *mNext{nullptr};
    Body *mBody{nullptr};
    float mFriction{0.0f};
    float mRestitution{0.0f};
    FixtureProxy *mProxies{nullptr};
    uint32_t mProxyCount{0};
    CollisionFilter mFilterData{};
    bool mIsSensor{false};
};


#endif //ENGINE3D_SRC_PHYSICS_FIXTURE_HPP
