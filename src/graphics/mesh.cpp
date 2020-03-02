//
// Created by Washington on 25/02/2020.
//

#include "mesh.hpp"

#include <memory>
#include "meshbuilder.hpp"
#include "opengl.hpp"

Mesh::Mesh(MeshBuilder *builder) {
    vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();

    if((builder->mAttributeBits & ATTRIBUTE_POSITION_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(builder->mPositions.size()*sizeof(builder->mPositions[0]), builder->mPositions.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_POSITION_INDEX);
        vertexArray->SetAttribPointer(ATTRIBUTE_POSITION_INDEX, 3, GL_FLOAT, false);
        buffers.emplace_back(buffer);
    }

    if((builder->mAttributeBits & ATTRIBUTE_NORMAL_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(builder->mNormals.size()*sizeof(builder->mNormals[0]), builder->mNormals.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_NORMAL_INDEX);
        vertexArray->SetAttribPointer(ATTRIBUTE_NORMAL_INDEX, 3, GL_FLOAT, false);
        buffers.emplace_back(buffer);
    }

    if((builder->mAttributeBits & ATTRIBUTE_TEXCOORD_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        if(builder->mFlipTextureCoordX) {
            for(size_t i = 0; i < builder->mTexCoords.size(); i+=2) {
                builder->mTexCoords[i] = 1.0f - builder->mTexCoords[i];
            }
        }
        if(builder->mFlipTextureCoordY) {
            for(size_t i = 1; i < builder->mTexCoords.size(); i+=2) {
                builder->mTexCoords[i] = 1.0f - builder->mTexCoords[i];
            }
        }
        buffer->SetData(builder->mTexCoords.size()*sizeof(builder->mTexCoords[0]), builder->mTexCoords.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_TEXCOORD_INDEX);
        vertexArray->SetAttribPointer(ATTRIBUTE_TEXCOORD_INDEX, 2, GL_FLOAT, false);
        buffers.emplace_back(buffer);
    }

    if((builder->mAttributeBits & ATTRIBUTE_JOINTS_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(builder->mJoints.size()*sizeof(builder->mJoints[0]), builder->mJoints.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_JOINTS_INDEX);
        vertexArray->SetAttribIPointer(ATTRIBUTE_JOINTS_INDEX, 4, GL_INT);
        buffers.emplace_back(buffer);
    }

    if((builder->mAttributeBits & ATTRIBUTE_WEIGHTS_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(builder->mWeights.size()*sizeof(builder->mWeights[0]), builder->mWeights.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_WEIGHTS_INDEX);
        vertexArray->SetAttribPointer(ATTRIBUTE_WEIGHTS_INDEX, 4, GL_FLOAT, false);
        buffers.emplace_back(buffer);
    }

    if((builder->mAttributeBits & ATTRIBUTE_TANGENT_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(builder->mTangents.size()*sizeof(builder->mTangents[0]), builder->mTangents.data(), GL_STATIC_DRAW);
        vertexArray->EnableAttrib(ATTRIBUTE_TANGENT_INDEX);
        vertexArray->SetAttribPointer(ATTRIBUTE_TANGENT_INDEX, 4, GL_FLOAT, false);
        buffers.emplace_back(buffer);
    }

    auto buffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
    buffer->Bind();
    buffer->SetData(builder->mIndices.size()*sizeof(builder->mIndices[0]), builder->mIndices.data(), GL_STATIC_DRAW);
    buffers.emplace_back(buffer);

    vertexArray->Unbind();

    renderMode = builder->mRenderMode;
    indexType = GL_UNSIGNED_INT;
    indexCount = builder->mIndices.size();
    material = builder->mMaterial;
    attributeBits = builder->mAttributeBits;
}
