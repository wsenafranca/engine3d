//
// Created by Washington on 14/03/2020.
//

#include "gizmos.hpp"
#include "opengl.hpp"
#include "shaderprogram.hpp"

void TGizmos::Init() {
    mLineBuffer = std::make_unique<Buffer>(GL_ARRAY_BUFFER);
    mLineBuffer->Bind();
    mLineBuffer->SetData(GIZMOS_BUFFER_SIZE*sizeof(VertexData), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(VertexData), (void*)(sizeof(float)*3));

    mLineBuffer->Unbind();
}

void TGizmos::Begin(Camera *camera, Shader* shader) {
    pCamera = camera;
    pShader = shader;
}

void TGizmos::DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec4 &color) {
    mLineData[mNumLines].pos = from;
    mLineData[mNumLines].color = color;
    ++mNumLines;
    mLineData[mNumLines].pos = to;
    mLineData[mNumLines].color = color;
    if(++mNumLines >= GIZMOS_BUFFER_SIZE) {
        Flush();
    }
}

void TGizmos::Flush() {
    if(mNumLines == 0) return;
    pShader->GetProgram()->Use();
    pShader->SetCamera(*pCamera);
    mLineBuffer->Bind();
    mLineBuffer->SetSubData(0, mNumLines*sizeof(VertexData), mLineData.data());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_LINES, 0, mNumLines);
    glEnable(GL_CULL_FACE);
    mLineBuffer->Unbind();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    mNumLines = 0;
}

void TGizmos::End() {
    Flush();
    pCamera = nullptr;
    pShader = nullptr;
}
