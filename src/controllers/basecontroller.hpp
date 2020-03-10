//
// Created by Washington on 07/03/2020.
//

#ifndef ENGINE3D_SRC_CONTROLLERS_BASECONTROLLER_HPP
#define ENGINE3D_SRC_CONTROLLERS_BASECONTROLLER_HPP

#include <base/object.hpp>

template <class T>
class BaseController : public Object {
public:
    virtual void Update(float dt) = 0;

    [[nodiscard]] const std::shared_ptr<T> &GetTarget() const {
        return mTarget;
    }

    virtual void SetTarget(const std::shared_ptr<T> &target) {
        mTarget = target;
    }

protected:
    std::shared_ptr<T> mTarget;
};


#endif //ENGINE3D_SRC_CONTROLLERS_BASECONTROLLER_HPP
