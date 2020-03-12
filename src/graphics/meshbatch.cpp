//
// Created by Washington on 24/02/2020.
//

#include "meshbatch.hpp"
#include "opengl.hpp"
#include <scenes/scene.hpp>
#include "shader.hpp"
#include "shaderprogram.hpp"

void MeshBatch::Begin(Scene* scene, Shader *shader) {
    pScene = scene;
    pShader = shader;
}

void MeshBatch::Render(Mesh *mesh, const glm::mat4 &worldMatrix, const std::vector<glm::mat4> &boneMatrices) {
    RenderCommand cmd;
    cmd.mesh = mesh;
    cmd.worldMatrix = worldMatrix;
    cmd.boneMatrices = boneMatrices;
    mCommands.push(cmd);
}

void MeshBatch::Render(Mesh *mesh, const glm::mat4 &worldMatrix) {
    RenderCommand cmd;
    cmd.mesh = mesh;
    cmd.worldMatrix = worldMatrix;
    mCommands.push(cmd);
}

void MeshBatch::Flush() {
    pShader->GetProgram()->Use();
    auto camLock = pScene->GetActiveCamera().lock();
    if(camLock) {
        pShader->SetCamera(*camLock);
    }
    pShader->SetLights(pScene->GetLights());
    while(!mCommands.empty()) {
        const auto& cmd = mCommands.front();
        pShader->SetWorldMatrix(cmd.worldMatrix);
        const auto& mesh = cmd.mesh;
        pShader->SetBoneMatrices(cmd.boneMatrices);
        pShader->SetMaterial(*mesh->material);
        pShader->SetAttributeBits(mesh->attributeBits);
        mesh->vertexArray->Bind();
        if(!mesh->material->backfaceCulling) {
            glDisable(GL_CULL_FACE);
        }
        glDrawElementsBaseVertex(mesh->renderMode, mesh->indexCount, mesh->indexType, nullptr, 0);
        if(!mesh->material->backfaceCulling) {
            glEnable(GL_CULL_FACE);
        }
        mesh->vertexArray->Unbind();
        mCommands.pop();
    }
}

void MeshBatch::End() {
    Flush();
    pScene = nullptr;
    pShader = nullptr;
}
