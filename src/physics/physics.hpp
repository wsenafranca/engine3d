//
// Created by Washington on 14/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_PHYSICS_HPP
#define ENGINE3D_SRC_PHYSICS_PHYSICS_HPP

#include <glm/glm.hpp>
#include <base/singleton.hpp>
#include "body.hpp"
#include "raycast.hpp"
#include <functional>
#include <btBulletDynamicsCommon.h>
#include "debugdraw.hpp"
#include "character.hpp"

class TPhysics {
public:
    void CreateWorld();
    Body* CreateBodyBox(const glm::vec3 &extent, float mass);
    Body* CreateBodySphere(float radius, float mass);
    Body* CreateBodyCapsule(const glm::vec3 &center, float radius, float height, float mass);
    Character* CreateCharacter(const glm::vec3 &center, float radius, float height);
    void DestroyBody(Body *body);
    void DestroyCharacter(Character *character);
    void DestroyWorld();

    void Update(float dt);
    bool RayCast(const glm::vec3 &from, const glm::vec3 &to, const std::function<float(const RayCastHit&)> &callback, unsigned short mask = 0xFFFF) const;
    void DrawDebug();

private:
    btDiscreteDynamicsWorld *mWorld{nullptr};
    btDefaultCollisionConfiguration* mCollisionConfiguration{nullptr};
    btCollisionDispatcher* mDispatcher{nullptr};
    btBroadphaseInterface* mOverlappingPairCache{nullptr};
    btSequentialImpulseConstraintSolver* mSolver{nullptr};
    DebugDraw *mDebugDraw{nullptr};
};

namespace Physics {
    static float FIXED_DELTA_TIME = 1.0f/60.0f;
    inline void CreateWorld() {
        Singleton<TPhysics>::GetInstance()->CreateWorld();
    }

    inline Body* CreateBodyBox(const glm::vec3 &extent, float mass) {
        return Singleton<TPhysics>::GetInstance()->CreateBodyBox(extent, mass);
    }

    inline Body* CreateBodySphere(float radius, float mass) {
        return Singleton<TPhysics>::GetInstance()->CreateBodySphere(radius, mass);
    }

    inline Body* CreateBodyCapsule(const glm::vec3 &center, float radius, float height, float mass) {
        return Singleton<TPhysics>::GetInstance()->CreateBodyCapsule(center, radius, height, mass);
    }

    inline Character* CreateCharacter(const glm::vec3 &center, float radius, float height) {
        return Singleton<TPhysics>::GetInstance()->CreateCharacter(center, radius, height);
    }

    inline void DestroyBody(Body *body) {
        Singleton<TPhysics>::GetInstance()->DestroyBody(body);
    }

    inline void DestroyCharacter(Character *character) {
        Singleton<TPhysics>::GetInstance()->DestroyCharacter(character);
    }

    inline void DestroyWorld() {
        Singleton<TPhysics>::GetInstance()->DestroyWorld();
    }

    inline void Update(float dt) {
        Singleton<TPhysics>::GetInstance()->Update(dt);
    }

    inline void RayCast(const glm::vec3 &point, const glm::vec3 &direction, const std::function<float(const RayCastHit&)> &callback, unsigned short mask = 0xFFFF) {
        Singleton<TPhysics>::GetInstance()->RayCast(point, direction, callback, mask);
    }

    inline void DrawDebug() {
        Singleton<TPhysics>::GetInstance()->DrawDebug();
    }
}

#endif //ENGINE3D_SRC_PHYSICS_PHYSICS_HPP
