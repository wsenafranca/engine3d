//
// Created by Washington on 24/02/2020.
//

#include "meshbatch.hpp"
#include "opengl.hpp"

void MeshBatch::Begin(Camera *camera, Shader *shader) {
    pCamera = camera;
    pShader = shader;
}

void MeshBatch::Render(Mesh *mesh, const glm::mat4 &worldMatrix, const std::vector<glm::mat4> *boneMatrices) {
    mCommands.push({mesh, worldMatrix, boneMatrices});
}

void MeshBatch::Flush() {
    pShader->GetProgram()->Use();
    pShader->SetCamera(pCamera);
    if(pLightSystem) {
        std::list<Light*> lights;
        for(const auto& it : pLightSystem->GetLights()) {
            lights.push_back(it.second.get());
        }
        pShader->SetLights(lights);
    }
    while(!mCommands.empty()) {
        const auto& cmd = mCommands.front();
        pShader->SetWorldMatrix(cmd.worldMatrix);
        const auto& mesh = cmd.mesh;
        if(cmd.boneMatrices) {
            pShader->SetBoneMatrices(*cmd.boneMatrices);
        }
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
    pCamera = nullptr;
    pShader = nullptr;
}

void MeshBatch::SetLightSystem(LightSystem *lightSystem) {
    pLightSystem = lightSystem;
}

LightSystem *MeshBatch::GetLightSystem() const {
    return pLightSystem;
}
