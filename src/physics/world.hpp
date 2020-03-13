//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_WORLD_HPP
#define ENGINE3D_SRC_PHYSICS_WORLD_HPP

#include "body.hpp"
#include "broadphase.hpp"
#include "raycast.hpp"
#include <list>

class World {
public:
    explicit World(const glm::vec3 &gravity);
    ~World();

    std::shared_ptr<Body> CreateBody(const glm::vec3 &center, const glm::vec3 &extents);
    void DestroyBody(const std::shared_ptr<Body> &body);

    void Update(float dt);

    [[nodiscard]] const glm::vec3 &GetGravity() const;
    void SetGravity(const glm::vec3 &gravity);

    [[nodiscard]] const std::list<std::shared_ptr<Body>> &GetBodyList() const;

private:
    glm::vec3 mGravity;
    std::list<std::shared_ptr<Body>> mBodyList;
    BroadPhase mBroadPhase;
};


#endif //ENGINE3D_SRC_PHYSICS_WORLD_HPP
