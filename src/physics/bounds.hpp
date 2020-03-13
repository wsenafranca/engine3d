//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_BOUNDS_HPP
#define ENGINE3D_SRC_PHYSICS_BOUNDS_HPP

#include <glm/glm.hpp>
#include "ray.hpp"

struct Bounds {
    glm::vec3 lowerBound{0.0f}, upperBound{0.0f};

    Bounds() = default;
    Bounds(const Bounds& rhs) = default;

    [[nodiscard]] glm::vec3 GetCenter() const;
    [[nodiscard]] glm::vec3 GetExtents() const;
    [[nodiscard]] glm::vec3 GetSize() const;
    [[nodiscard]] float GetPerimeter() const;

    [[nodiscard]] bool operator==(const Bounds &rhs) const;
    [[nodiscard]] bool operator!=(const Bounds &rhs) const;
};

inline Bounds operator+(const Bounds &a, const Bounds &b) {
    Bounds result;
    result.lowerBound = (glm::min)(a.lowerBound, b.lowerBound);
    result.upperBound = (glm::max)(a.upperBound, b.upperBound);
    return result;
}

inline Bounds operator+(const Bounds &a, const glm::vec3 &b) {
    Bounds result;
    result.lowerBound = (glm::min)(a.lowerBound, b);
    result.upperBound = (glm::max)(a.upperBound, b);
    return result;
}

inline Bounds operator+(const glm::vec3 &a, const Bounds &b) {
    Bounds result;
    result.lowerBound = (glm::min)(b.lowerBound, a);
    result.upperBound = (glm::max)(b.upperBound, a);
    return result;
}

inline Bounds& operator+=(Bounds &a, const Bounds &b) {
    a.lowerBound = (glm::min)(a.lowerBound, b.lowerBound);
    a.upperBound = (glm::max)(a.upperBound, b.upperBound);
    return a;
}

inline Bounds& operator+=(Bounds &a, const glm::vec3 &b) {
    a.lowerBound = (glm::min)(a.lowerBound, b);
    a.upperBound = (glm::max)(a.upperBound, b);
    return a;
}

namespace Physics {
    inline bool Contains(const Bounds &a, const Bounds &b) {
        bool result = true;
        result = result && a.lowerBound.x <= b.lowerBound.x;
        result = result && a.lowerBound.y <= b.lowerBound.y;
        result = result && b.upperBound.x <= a.upperBound.x;
        result = result && b.upperBound.y <= a.upperBound.y;
        return result;
    }

    inline bool Contains(const Bounds &a, const glm::vec3 &b) {
        return  (b.x >= a.lowerBound.x && b.x <= a.upperBound.x) &&
                (b.y >= a.lowerBound.y && b.y <= a.upperBound.y) &&
                (b.z >= a.lowerBound.z && b.z <= a.upperBound.z);
    }

    inline bool Intersects(const Bounds &a, const Bounds &b) {
        glm::vec3 d1, d2;
        d1 = b.lowerBound - a.upperBound;
        d2 = a.lowerBound - b.upperBound;
        return  !((d1.x > 0.0f || d1.y > 0.0f || d1.z > 0.0f)) &&
                !((d2.x > 0.0f || d2.y > 0.0f || d2.z > 0.0f));
    }

    inline bool IntersectRay(const Bounds &a, const Ray &b) {
        const glm::vec3 point2 = b.point1 + b.maxFraction * (b.point2 - b.point1);
        const glm::vec3 e = a.upperBound - a.lowerBound;
        const glm::vec3 d = point2 - b.point1;
        const glm::vec3 m = b.point1 + point2 - a.lowerBound - a.upperBound;

        float adx = std::abs(d.x);
        if (std::abs(m.x) > e.x + adx) return false;
        float ady = std::abs(d.y);
        if (std::abs(m.y) > e.y + ady) return false;
        float adz = std::abs(d.z);
        if (std::abs(m.z) > e.z + adz) return false;

        const float epsilon = 0.00001;
        adx += epsilon;
        ady += epsilon;
        adz += epsilon;

        if (std::abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;
        if (std::abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;
        if (std::abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;

        return true;
    }


}

#endif //ENGINE3D_SRC_PHYSICS_BOUNDS_HPP
