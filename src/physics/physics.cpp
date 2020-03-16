//
// Created by Washington on 14/03/2020.
//

#include "physics.hpp"
#include <glm/gtc/type_ptr.hpp>

void TPhysics::CreateWorld() {
    mCollisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    mOverlappingPairCache = new btDbvtBroadphase();
    mOverlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    mSolver = new btSequentialImpulseConstraintSolver;

    mWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);

    mDebugDraw = new DebugDraw();
    mDebugDraw->setDebugMode(DebugDraw::DBG_DrawWireframe);
    mWorld->setDebugDrawer(mDebugDraw);
}

Body *TPhysics::CreateBodyBox(const glm::vec3 &extent, float mass) {
    auto shape = new btBoxShape(btVector3(extent.x, extent.y, extent.z));
    bool isDynamic = mass != 0.f;
    btVector3 localInertia(0, 0, 0);
    if(isDynamic) {
        shape->calculateLocalInertia(mass, localInertia);
    }

    btTransform transform;
    transform.setIdentity();
    auto motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);
    auto body = new btRigidBody(info);
    mWorld->addRigidBody(body);

    auto newBody = new Body();
    body->setUserPointer(this);
    newBody->mBody = body;
    return newBody;
}

Body *TPhysics::CreateBodySphere(float radius, float mass) {
    auto shape = new btSphereShape(radius);
    bool isDynamic = mass != 0.f;
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(isDynamic) {
        shape->calculateLocalInertia(mass, localInertia);
    }

    btTransform transform;
    transform.setIdentity();
    auto motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);
    auto body = new btRigidBody(info);
    mWorld->addRigidBody(body);

    auto newBody = new Body();
    body->setUserPointer(this);
    newBody->mBody = body;
    return newBody;
}

Body *TPhysics::CreateBodyCapsule(const glm::vec3 &center, float radius, float height, float mass) {
    auto shape = new btCompoundShape();
    shape->addChildShape(
            btTransform(btQuaternion::getIdentity(), btVector3(center.x, center.y, center.z)),
            new btCapsuleShape(radius, height));
    bool isDynamic = mass != 0.f;
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if(isDynamic) {
        shape->calculateLocalInertia(mass, localInertia);
    }

    btTransform transform;
    transform.setIdentity();
    auto motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);
    auto body = new btRigidBody(info);
    mWorld->addRigidBody(body);

    auto newBody = new Body();
    body->setUserPointer(this);
    newBody->mBody = body;
    return newBody;
}

Character *TPhysics::CreateCharacter(const glm::vec3 &center, float radius, float height) {
    auto shape = new btCapsuleShape(radius, height);

    auto ghostBody = new btPairCachingGhostObject();
    ghostBody->setCollisionShape(shape);
    ghostBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    ghostBody->setUserPointer(this);

    auto controller = new btKinematicCharacterController(ghostBody, shape, 0.3f, btVector3(0.0f, 1.0f, 0.0f));
    controller->setUseGhostSweepTest(false);

    mWorld->addCollisionObject(ghostBody, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    mWorld->addCharacter(controller);

    auto newCharacter = new Character();
    newCharacter->SetOffset(-center);
    newCharacter->mCharacterController = controller;
    return newCharacter;
}

void TPhysics::DestroyBody(Body *body) {
    mWorld->removeRigidBody(body->mBody);
    if (body->mBody->getMotionState()) {
        delete body->mBody->getMotionState();
    }
    delete body->mBody;
}

void TPhysics::DestroyCharacter(Character *character) {
    mWorld->removeCollisionObject(character->mCharacterController->getGhostObject());
    mWorld->removeCharacter(character->mCharacterController);
    delete character->mCharacterController->getGhostObject();
    delete character->mCharacterController;
}

void TPhysics::DestroyWorld() {
    delete mDebugDraw;
    //delete dynamics world
	delete mWorld;

	//delete solver
	delete mSolver;

	//delete broadphase
	delete mOverlappingPairCache;

	//delete dispatcher
	delete mDispatcher;

	delete mCollisionConfiguration;
}

void TPhysics::Update(float) {
    mWorld->stepSimulation(1.f / 60.f, 10);
}

bool TPhysics::RayCast(const glm::vec3 &from, const glm::vec3 &to,
                       const std::function<float(const RayCastHit &)> &callback,
                       unsigned short mask) const {
    struct Callback : public btCollisionWorld::RayResultCallback {
        std::function<float(const RayCastHit &)> mCallback;
        btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) override {
            RayCastHit hit{};
            hit.normal = glm::make_vec3(&rayResult.m_hitNormalLocal.x());
            hit.body = (Body*)rayResult.m_collisionObject->getUserPointer();
            hit.fraction = rayResult.m_hitFraction;
            return mCallback(hit);
        }
    };

    auto cb = Callback{};
    cb.mCallback = callback;
    mWorld->rayTest(btVector3(from.x, from.y, from.z), btVector3(to.x, to.y, to.z), cb);
    return cb.hasHit();
}

void TPhysics::DrawDebug() {
    mWorld->debugDrawWorld();
}
