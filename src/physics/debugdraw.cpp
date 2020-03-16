//
// Created by Washington on 14/03/2020.
//

#include "debugdraw.hpp"
#include <graphics/gizmos.hpp>

void DebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    Gizmos::DrawLine(
            glm::vec3(from.x(), from.y(), from.z()),
            glm::vec3(to.x(), to.y(), to.z()),
            glm::vec4(color.x(), color.y(), color.z(), 1.0f));
}

void DebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                                 const btVector3 &color) {

}

void DebugDraw::reportErrorWarning(const char *warningString) {

}

void DebugDraw::draw3dText(const btVector3 &location, const char *textString) {

}

void DebugDraw::setDebugMode(int debugMode) {
    mDebugMode = debugMode;
}

int DebugDraw::getDebugMode() const {
    return mDebugMode;
}
