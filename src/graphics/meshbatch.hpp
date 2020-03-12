//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_GRAPHICS_MESHBATCH_HPP
#define ENGINE3D_SRC_GRAPHICS_MESHBATCH_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include <queue>

class Scene;

class MeshBatch {
public:
    void Begin(Scene* scene, Shader* shader);
    void Render(Mesh* mesh, const glm::mat4& worldMatrix, const std::vector<glm::mat4> &boneMatrices);
    void Render(Mesh* mesh, const glm::mat4& worldMatrix);
    void Flush();
    void End();

private:
    Scene *pScene{nullptr};
    Shader *pShader{nullptr};
    struct RenderCommand {
        const Mesh* mesh;
        glm::mat4 worldMatrix;
        std::vector<glm::mat4> boneMatrices;
    };
    std::queue<RenderCommand> mCommands;
};

#endif //ENGINE3D_SRC_GRAPHICS_MESHBATCH_HPP
