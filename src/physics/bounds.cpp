//
// Created by Washington on 12/03/2020.
//

#include "bounds.hpp"

glm::vec3 Bounds::GetCenter() const {
    return (lowerBound+upperBound)*0.5f;
}

glm::vec3 Bounds::GetExtents() const {
    return (lowerBound-upperBound)*0.5f;
}

glm::vec3 Bounds::GetSize() const {
    return (lowerBound-upperBound);
}

float Bounds::GetPerimeter() const {
    float wx = upperBound.x - lowerBound.x;
    float wy = upperBound.y - lowerBound.y;
    float wz = upperBound.z - lowerBound.z;
    return 2.0f*(wx + wy + wz);
}

bool Bounds::operator==(const Bounds &rhs) const {
    return lowerBound == rhs.lowerBound &&
           upperBound == rhs.upperBound;
}

bool Bounds::operator!=(const Bounds &rhs) const {
    return !(rhs == *this);
}
