//
// Created by Washington on 14/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_DEBUGDRAW_HPP
#define ENGINE3D_SRC_PHYSICS_DEBUGDRAW_HPP

#include <LinearMath/btIDebugDraw.h>

class DebugDraw : public btIDebugDraw {
public:
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                          const btVector3 &color) override;

    void reportErrorWarning(const char *warningString) override;

    void draw3dText(const btVector3 &location, const char *textString) override;

    void setDebugMode(int debugMode) override;
    [[nodiscard]] int getDebugMode() const override;

private:
    int mDebugMode{0};
};


#endif //ENGINE3D_SRC_PHYSICS_DEBUGDRAW_HPP
