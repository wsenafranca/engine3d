//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_GRAPHICS_SHADER_HPP
#define ENGINE3D_SRC_GRAPHICS_SHADER_HPP

#include <math/transform.hpp>
#include <vector>

class Camera;
class Light;
class Material;
class ShaderProgram;

class Shader {
public:
    virtual ~Shader() = default;
    [[nodiscard]] virtual ShaderProgram* GetProgram() const = 0;
    virtual void SetCamera(const Camera &camera) = 0;
    virtual void SetLights(const std::vector< std::weak_ptr<Light> >& lights) = 0;
    virtual void SetWorldMatrix(const glm::mat4& worldMatrix) = 0;
    virtual void SetMaterial(const Material& material) = 0;
    virtual void SetAttributeBits(uint32_t attributeBits) = 0;
    virtual void SetBoneMatrices(const std::vector<glm::mat4>& matrices) = 0;
};


#endif //ENGINE3D_SRC_GRAPHICS_SHADER_HPP
