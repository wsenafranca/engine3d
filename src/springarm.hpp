//
// Created by Washington on 02/03/2020.
//

#ifndef ENGINE3D_SRC_SPRINGARM_HPP
#define ENGINE3D_SRC_SPRINGARM_HPP

class SpringArm {
public:


private:
    float mTargetArmLength{0.5f};
    float mProbeSize{1.0f};
    bool mInheritPitch{true};
    bool mInheritRoll{true};
    bool mInheritYaw{true};
};

#endif //ENGINE3D_SRC_SPRINGARM_HPP
