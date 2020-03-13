//
// Created by Washington on 13/03/2020.
//

#include "fixture.hpp"
#include "body.hpp"
#include "world.hpp"
#include "broadphase.hpp"

Fixture::Fixture(Body *body, const FixtureDef *def) {
    mFriction = def->friction;
    mRestitution = def->restitution;
    mBody = body;
    mNext = nullptr;
    mFilterData = def->filter;
    mIsSensor = def->isSensor;
    mProxies = (FixtureProxy*)malloc(sizeof(FixtureProxy));
    mProxies->fixture = nullptr;
    mProxies->proxyId = -1;
    mDensity = def->density;
}

Fixture::~Fixture() {
    assert(mProxyCount == 0);

    // Free the proxy array.
    free(mProxies);
    mProxies = nullptr;
}

void Fixture::CreateProxies(BroadPhase *broadPhase, const Transform &xf) {
    assert(mProxyCount == 0);

    mProxyCount = 1;
    mProxies->proxyId = 0;
    mProxies->fixture = this;
    mProxies->childIndex = 0;
}

void Fixture::DestroyProxies(BroadPhase *broadPhase) {

}

void Fixture::Synchronize(BroadPhase *broadPhase, const Transform &xf1, const Transform &xf2) {

}

float Fixture::GetDensity() const {
    return mDensity;
}

void Fixture::SetDensity(float density) {
    mDensity = density;
}

Fixture *Fixture::GetNext() const {
    return mNext;
}

Body *Fixture::GetBody() const {
    return mBody;
}

float Fixture::GetFriction() const {
    return mFriction;
}

void Fixture::SetFriction(float friction) {
    mFriction = friction;
}

float Fixture::GetRestitution() const {
    return mRestitution;
}

void Fixture::SetRestitution(float restitution) {
    mRestitution = restitution;
}

FixtureProxy *Fixture::GetProxies() const {
    return mProxies;
}

void Fixture::SetProxies(FixtureProxy *proxies) {
    mProxies = proxies;
}

uint32_t Fixture::GetProxyCount() const {
    return mProxyCount;
}

void Fixture::SetProxyCount(uint32_t proxyCount) {
    mProxyCount = proxyCount;
}

const CollisionFilter &Fixture::GetFilterData() const {
    return mFilterData;
}

void Fixture::SetFilterData(const CollisionFilter &filterData) {
    mFilterData = filterData;
}

bool Fixture::IsSensor() const {
    return mIsSensor;
}

void Fixture::SetIsSensor(bool isSensor) {
    mIsSensor = isSensor;
}

bool Fixture::TestPoint(const glm::vec3 &p) const {
    return false;
}

bool Fixture::RayCast(RayCastHit &hit, const Ray &ray) const {
    return false;
}

const Bounds &Fixture::GetBounds() const {
    assert(mProxyCount >= 0);
    return mProxies->bounds;
}
