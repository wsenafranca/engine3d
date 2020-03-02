//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_MESHBATCH_HPP
#define ENGINE3D_SRC_MESHBATCH_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "../components/lightsystem.hpp"
#include <queue>

class MeshBatch {
public:
    void Begin(Camera* camera, Shader* shader);
    void Render(Mesh* mesh, const glm::mat4& worldMatrix, const std::vector<glm::mat4>* boneMatrices = nullptr);
    void Flush();
    void End();

    void SetLightSystem(LightSystem *lightSystem);
    LightSystem* GetLightSystem() const;
private:
    Camera *pCamera{nullptr};
    Shader *pShader{nullptr};
    LightSystem *pLightSystem{nullptr};
    struct RenderCommand {
        const Mesh* mesh;
        glm::mat4 worldMatrix;
        const std::vector<glm::mat4>* boneMatrices;
    };
    std::queue<RenderCommand> mCommands;
};

#endif //ENGINE3D_SRC_MESHBATCH_HPP
