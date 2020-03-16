//
// Created by Washington on 14/03/2020.
//

#ifndef ENGINE3D_SRC_GRAPHICS_GIZMOS_HPP
#define ENGINE3D_SRC_GRAPHICS_GIZMOS_HPP

#include <array>
#include <components/camera.hpp>
#include <memory>
#include "buffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include <base/singleton.hpp>

#define GIZMOS_BUFFER_SIZE 1024

class TGizmos {
public:
    void Init();
    void Begin(Camera *camera, Shader* shader);
    void DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec4 &color);
    void Flush();
    void End();

private:
    Camera *pCamera{nullptr};
    Shader* pShader{nullptr};
    struct VertexData {
        glm::vec3 pos;
        glm::vec4 color;
    };
    std::array<VertexData, GIZMOS_BUFFER_SIZE> mLineData;
    std::unique_ptr<Buffer> mLineBuffer;
    size_t mNumLines{0};
};

namespace Gizmos {
    inline void Init() {Singleton<TGizmos>::GetInstance()->Init();}
    inline void Begin(Camera *camera, Shader* shader) {Singleton<TGizmos>::GetInstance()->Begin(camera, shader);}
    inline void DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec4 &color) {
        Singleton<TGizmos>::GetInstance()->DrawLine(from, to, color);
    }
    inline void Flush() {Singleton<TGizmos>::GetInstance()->Flush();}
    inline void End() {Singleton<TGizmos>::GetInstance()->End();}
}

#endif //ENGINE3D_SRC_GRAPHICS_GIZMOS_HPP
