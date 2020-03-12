//
// Created by Washington on 25/02/2020.
//

#include "meshbuilder.hpp"
#include <numeric>
#include "opengl.hpp"

MeshBuilder &MeshBuilder::SetPositions(const std::vector<float> &positions) {
    mPositions = positions;
    mAttributeBits |= ATTRIBUTE_POSITION_BITS;
    return *this;
}

const std::vector<float> &MeshBuilder::GetPositions() const {
    return mPositions;
}

MeshBuilder &MeshBuilder::SetNormals(const std::vector<float> &normals) {
    mNormals = normals;
    mAttributeBits |= ATTRIBUTE_NORMAL_BITS;
    return *this;
}

const std::vector<float> &MeshBuilder::GetNormals() const {
    return mNormals;
}

MeshBuilder &MeshBuilder::SetTexCoords(const std::vector<float> &texCoords) {
    mTexCoords = texCoords;
    mAttributeBits |= ATTRIBUTE_TEXCOORD_BITS;
    return *this;
}

const std::vector<float> &MeshBuilder::GetTexCoords() const {
    return mTexCoords;
}

MeshBuilder &MeshBuilder::SetJoints(const std::vector<int> &joints) {
    mJoints = joints;
    mAttributeBits |= ATTRIBUTE_JOINTS_BITS;
    return *this;
}

const std::vector<int> &MeshBuilder::GetJoints() const {
    return mJoints;
}

MeshBuilder &MeshBuilder::SetWeights(const std::vector<float> &weights) {
    mWeights = weights;
    mAttributeBits |= ATTRIBUTE_WEIGHTS_BITS;
    return *this;
}

const std::vector<float> &MeshBuilder::GetWeights() const {
    return mWeights;
}

MeshBuilder &MeshBuilder::SetTangents(const std::vector<float> &tangents) {
    mTangents = tangents;
    mAttributeBits |= ATTRIBUTE_TANGENT_BITS;
    return *this;
}

const std::vector<float> &MeshBuilder::GetTangents() const {
    return mTangents;
}

MeshBuilder &MeshBuilder::SetIndices(const std::vector<uint32_t> &indices) {
    mIndices = indices;
    return *this;
}

const std::vector<uint32_t> &MeshBuilder::GetIndices() const {
    return mIndices;
}

MeshBuilder &MeshBuilder::SetMaterial(const std::shared_ptr<Material> &material) {
    mMaterial = material;
    return *this;
}

const std::shared_ptr<Material> &MeshBuilder::GetMaterial() const {
    return mMaterial;
}

MeshBuilder &MeshBuilder::SetSkeleton(const std::shared_ptr<Skeleton> &skeleton) {
    mSkeleton = skeleton;
    return *this;
}

const std::shared_ptr<Skeleton> &MeshBuilder::GetSkeleton() const {
    return mSkeleton;
}

MeshBuilder &MeshBuilder::SetFlipTextureCoord(bool flipX, bool flipY) {
    mFlipTextureCoordX = flipX;
    mFlipTextureCoordY = flipY;
    return *this;
}

bool MeshBuilder::IsFlipTextureCoordX() const {
    return mFlipTextureCoordX;
}

bool MeshBuilder::IsFlipTextureCoordY() const {
    return mFlipTextureCoordY;
}

MeshBuilder &MeshBuilder::SetRenderMode(uint32_t mode) {
    mRenderMode = mode;
    return *this;
}

uint32_t MeshBuilder::GetRenderMode() const {
    return mRenderMode;
}

std::shared_ptr<Mesh> MeshBuilder::Build() {
    auto mesh = std::make_shared<Mesh>();

    mesh->vertexArray = std::make_unique<VertexArray>();
    mesh->vertexArray->Bind();

    if((mAttributeBits & ATTRIBUTE_POSITION_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(mPositions.size()*sizeof(mPositions[0]), mPositions.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_POSITION_INDEX);
        mesh->vertexArray->SetAttribPointer(ATTRIBUTE_POSITION_INDEX, 3, GL_FLOAT, false);
        mesh->buffers.emplace_back(buffer);
    }

    if((mAttributeBits & ATTRIBUTE_NORMAL_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(mNormals.size()*sizeof(mNormals[0]), mNormals.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_NORMAL_INDEX);
        mesh->vertexArray->SetAttribPointer(ATTRIBUTE_NORMAL_INDEX, 3, GL_FLOAT, false);
        mesh->buffers.emplace_back(buffer);
    }

    if((mAttributeBits & ATTRIBUTE_TEXCOORD_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        if(mFlipTextureCoordX) {
            for(size_t i = 0; i < mTexCoords.size(); i+=2) {
                mTexCoords[i] = 1.0f - mTexCoords[i];
            }
        }
        if(mFlipTextureCoordY) {
            for(size_t i = 1; i < mTexCoords.size(); i+=2) {
                mTexCoords[i] = 1.0f - mTexCoords[i];
            }
        }
        buffer->SetData(mTexCoords.size()*sizeof(mTexCoords[0]), mTexCoords.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_TEXCOORD_INDEX);
        mesh->vertexArray->SetAttribPointer(ATTRIBUTE_TEXCOORD_INDEX, 2, GL_FLOAT, false);
        mesh->buffers.emplace_back(buffer);
    }

    if((mAttributeBits & ATTRIBUTE_JOINTS_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(mJoints.size()*sizeof(mJoints[0]), mJoints.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_JOINTS_INDEX);
        mesh->vertexArray->SetAttribIPointer(ATTRIBUTE_JOINTS_INDEX, 4, GL_INT);
        mesh->buffers.emplace_back(buffer);
    }

    if((mAttributeBits & ATTRIBUTE_WEIGHTS_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(mWeights.size()*sizeof(mWeights[0]), mWeights.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_WEIGHTS_INDEX);
        mesh->vertexArray->SetAttribPointer(ATTRIBUTE_WEIGHTS_INDEX, 4, GL_FLOAT, false);
        mesh->buffers.emplace_back(buffer);
    }

    if((mAttributeBits & ATTRIBUTE_TANGENT_BITS)) {
        auto buffer = new Buffer(GL_ARRAY_BUFFER);
        buffer->Bind();
        buffer->SetData(mTangents.size()*sizeof(mTangents[0]), mTangents.data(), GL_STATIC_DRAW);
        mesh->vertexArray->EnableAttrib(ATTRIBUTE_TANGENT_INDEX);
        mesh->vertexArray->SetAttribPointer(ATTRIBUTE_TANGENT_INDEX, 4, GL_FLOAT, false);
        mesh->buffers.emplace_back(buffer);
    }

    auto buffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
    buffer->Bind();
    buffer->SetData(mIndices.size()*sizeof(mIndices[0]), mIndices.data(), GL_STATIC_DRAW);
    mesh->buffers.emplace_back(buffer);

    mesh->vertexArray->Unbind();

    mesh->renderMode = mRenderMode;
    mesh->indexType = GL_UNSIGNED_INT;
    mesh->indexCount = mIndices.size();
    mesh->material = mMaterial;
    mesh->attributeBits = mAttributeBits;
    mesh->skeleton = mSkeleton;

    return mesh;
}

MeshBuilder &MeshBuilder::SetQuad(float width, float height) {
    SetPlane(
            glm::vec3(-width*0.5f, -height*0.5f, 0.0f),
            glm::vec3( width*0.5f, -height*0.5f, 0.0f),
            glm::vec3( width*0.5f,  height*0.5f, 0.0f),
            glm::vec3(-width*0.5f,  height*0.5f, 0.0f));
    return *this;
}

MeshBuilder &MeshBuilder::SetBox(const glm::vec3& size) {
    glm::vec3 s = size*0.5f;
    mPositions = {
            // back
            -s.x, -s.y, -s.z,
             s.x,  s.y, -s.z,
             s.x, -s.y, -s.z,
             s.x,  s.y, -s.z,
            -s.x, -s.y, -s.z,
            -s.x,  s.y, -s.z,

            // front
            -s.x, -s.y,  s.z,
             s.x, -s.y,  s.z,
             s.x,  s.y,  s.z,
             s.x,  s.y,  s.z,
            -s.x,  s.y,  s.z,
            -s.x, -s.y,  s.z,

            // left
            -s.x,  s.y,  s.z,
            -s.x,  s.y, -s.z,
            -s.x, -s.y, -s.z,
            -s.x, -s.y, -s.z,
            -s.x, -s.y,  s.z,
            -s.x,  s.y,  s.z,

            // right
             s.x,  s.y,  s.z,
             s.x, -s.y, -s.z,
             s.x,  s.y, -s.z,
             s.x, -s.y, -s.z,
             s.x,  s.y,  s.z,
             s.x, -s.y,  s.z,

            // bottom
            -s.x, -s.y, -s.z,
             s.x, -s.y, -s.z,
             s.x, -s.y,  s.z,
             s.x, -s.y,  s.z,
            -s.x, -s.y,  s.z,
            -s.x, -s.y, -s.z,

            // top
            -s.x,  s.y, -s.z,
             s.x,  s.y,  s.z,
             s.x,  s.y, -s.z,
             s.x,  s.y,  s.z,
            -s.x,  s.y, -s.z,
            -s.x,  s.y,  s.z,
    };

    mNormals = {
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,

            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,

            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,

            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,

            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,

            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
    };

    mTexCoords = {
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 0.0f
    };
    mAttributeBits = ATTRIBUTE_POSITION_BITS|ATTRIBUTE_TEXCOORD_BITS|ATTRIBUTE_NORMAL_BITS;

    mIndices.resize(36);
    std::iota(mIndices.begin(), mIndices.end(), 0);

    return *this;
}

MeshBuilder &MeshBuilder::SetPlane(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &p4) {
    mPositions = {
            p1.x, p1.y, p1.z,
            p2.x, p2.y, p2.z,
            p3.x, p3.y, p3.z,
            p4.x, p4.y, p4.z
    };

    mTexCoords = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };

    mIndices = {
            0, 1, 2,
            2, 3, 0
    };

    std::vector<glm::vec3> vertices{p1, p2, p3, p4};
    std::vector<glm::vec3> normals{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)};

    for(size_t i = 0; i < mIndices.size(); i+=3) {
        glm::vec3 p = glm::cross(vertices[mIndices[i+1]] - vertices[mIndices[i]], vertices[mIndices[i+2]] - vertices[mIndices[i]]);
        normals[mIndices[i]] += p;
        normals[mIndices[i+1]] += p;
        normals[mIndices[i+2]] += p;
    }

    for(auto& normal : normals) {
        normal = glm::normalize(normal);
    }

    mNormals = std::vector<float>(12);
    memcpy(mNormals.data(), normals.data(), sizeof(mNormals[0])*mNormals.size());

    mAttributeBits = ATTRIBUTE_POSITION_BITS|ATTRIBUTE_TEXCOORD_BITS|ATTRIBUTE_NORMAL_BITS;

    return *this;
}
