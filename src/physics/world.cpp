//
// Created by Washington on 12/03/2020.
//

#include "world.hpp"

World::World(const glm::vec3 &gravity) : mGravity(gravity) {

}

World::~World() {

}

std::shared_ptr<Body> World::CreateBody(const glm::vec3 &center, const glm::vec3 &extents) {
    auto body = std::make_shared<Body>(center, extents);
    mBodyList.push_back(body);
    return body;
}

void World::DestroyBody(const std::shared_ptr<Body> &body) {
    if(!body) return;
    for(auto it = mBodyList.begin(); it != mBodyList.end(); ++it) {
        if(it->get() == body.get()) {
            mBodyList.erase(it);
            return;
        }
    }
}

void World::Update(float dt) {

}

const glm::vec3 &World::GetGravity() const {
    return mGravity;
}

void World::SetGravity(const glm::vec3 &gravity) {
    mGravity = gravity;
}

const std::list<std::shared_ptr<Body>> &World::GetBodyList() const {
    return mBodyList;
}

