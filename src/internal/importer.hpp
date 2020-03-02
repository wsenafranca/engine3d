//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_IMPORTER_HPP
#define ENGINE3D_SRC_IMPORTER_HPP

#include "../node.hpp"
#include "../model.hpp"
#include "../meshbuilder.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include "../texturebuilder.hpp"
#include "../opengl.hpp"

class Importer {
public:
    static void LoadModel(Model *model, const filesystem::path& file) {
        if(!file.exists()) {
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
        }

        Assimp::Importer importer;
        const aiScene *pScene = importer.ReadFile(file.str(),
                aiProcess_Triangulate |
                aiProcess_CalcTangentSpace |
                aiProcess_GenNormals |
                aiProcess_GenUVCoords |
                aiProcess_LimitBoneWeights |
                aiProcess_PopulateArmatureData |
                aiProcess_GlobalScale);
        if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
            throw std::runtime_error("Failed to import " + file.str() + importer.GetErrorString());
        }

        auto root = BuildNode(pScene->mRootNode);
        root->name = file.filename().substr(0, file.filename().rfind('.'));
        model->SetRootNode(root);

        for(size_t i = 0; i < pScene->mNumMeshes; i++) {
            LoadMesh(model, pScene, pScene->mMeshes[i]);
        }

        LoadNode(root, model, pScene->mRootNode, pScene);

        importer.FreeScene();
    }

    static void LoadAnimation(Animation* animation, const filesystem::path& file) {
        if(!file.exists()) {
            throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
        }

        Assimp::Importer importer;
        const aiScene *pScene = importer.ReadFile(file.str(),
                                                  aiProcess_LimitBoneWeights |
                                                  aiProcess_PopulateArmatureData |
                                                  aiProcess_GlobalScale);

        if(!pScene) {
            throw std::runtime_error("Failed to import " + file.str() + importer.GetErrorString());
        }

        if(pScene->mNumAnimations > 0) {
            auto pAnimation = pScene->mAnimations[0];
            LoadAnimation(animation, pAnimation);
        }

        importer.FreeScene();
    }

protected:
    static Node* BuildNode(const aiNode *pNode) {
        auto node = new Node();
        node->name = pNode->mName.C_Str();
        for(size_t i = 0; i < pNode->mNumChildren; i++) {
            node->children.push_back(BuildNode(pNode->mChildren[i]));
        }

        return node;
    }
    static void LoadNode(Node* node, Model *model, const aiNode *pNode, const aiScene *pScene) {
        for(size_t i = 0; i < pNode->mNumMeshes; i++) {
            node->meshes.push_back(model->GetMesh(pNode->mMeshes[i]));
        }

        aiMatrix4x4 transformation = pNode->mTransformation;
        node->matrix = glm::make_mat4(&transformation.Transpose().a1);

        uint32_t i = 0;
        for(auto& child : node->children) {
            LoadNode(child, model, pNode->mChildren[i], pScene);
            i++;
        }
    }

    static Material LoadMaterial(Model* model, const aiMaterial *pMaterial) {
        MaterialBuilder builder;

        aiColor3D outColor;
        float outFloat;
        int outInt;
        if(pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, outColor) == aiReturn_SUCCESS) {
            builder.SetDiffuseColor(glm::make_vec3(&outColor.r));
        }

        if(pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, outColor) == aiReturn_SUCCESS) {
            builder.SetSpecularColor(glm::make_vec3(&outColor.r));
        }

        if(pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, outColor) == aiReturn_SUCCESS) {
            builder.SetAmbientColor(glm::make_vec3(&outColor.r));
        }

        if(pMaterial->Get(AI_MATKEY_SHININESS, outFloat) == aiReturn_SUCCESS) {
            builder.SetShininess(outFloat);
        }

        if(pMaterial->Get(AI_MATKEY_OPACITY, outFloat) == aiReturn_SUCCESS) {
            builder.SetOpacity(outFloat);
        }

        if(pMaterial->Get(AI_MATKEY_TWOSIDED, outInt) == aiReturn_SUCCESS) {
            builder.SetBackfaceCulling(!outInt);
        }

        if(pMaterial->Get(AI_MATKEY_BLEND_FUNC, outInt) == aiReturn_SUCCESS) {
            builder.SetBackfaceCulling(outInt == aiBlendMode_Default ? MATERIAL_BLEND_MODE_DEFAULT : MATERIAL_BLEND_MODE_ADDITIVE);
        }

        filesystem::path textureDir("data/textures");
        aiString path;
        aiTextureMapMode textureMapMode;

        if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS ||
            pMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS)
        {
            filesystem::path file = textureDir/filesystem::path(path.C_Str()).filename();
            auto textureBuilder = TextureBuilder().SetTexture2D(file);
            if(textureMapMode == aiTextureMapMode_Clamp) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            } else if(textureMapMode == aiTextureMapMode_Wrap) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            } else if(textureMapMode == aiTextureMapMode_Mirror) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
            }
            auto texture = textureBuilder.Build();
            builder.SetDiffuseTexture(texture);
        }

        if(pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS) {
            filesystem::path file = textureDir/filesystem::path(path.C_Str()).filename();
            auto textureBuilder = TextureBuilder().SetTexture2D(file);
            if(textureMapMode == aiTextureMapMode_Clamp) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            else if(textureMapMode == aiTextureMapMode_Wrap) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if(textureMapMode == aiTextureMapMode_Mirror) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
            }
            auto texture = textureBuilder.Build();
            builder.SetSpecularTexture(texture);
        }

        if(pMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS ||
            pMaterial->GetTexture(aiTextureType_AMBIENT, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS)
        {
            filesystem::path file = textureDir/filesystem::path(path.C_Str()).filename();
            auto textureBuilder = TextureBuilder().SetTexture2D(file);
            if(textureMapMode == aiTextureMapMode_Clamp) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            else if(textureMapMode == aiTextureMapMode_Wrap) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if(textureMapMode == aiTextureMapMode_Mirror) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
            }
            auto texture = textureBuilder.Build();
            builder.SetOcclusionTexture(texture);
        }

        if(pMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS) {
            filesystem::path file = textureDir/filesystem::path(path.C_Str()).filename();
            auto textureBuilder = TextureBuilder().SetTexture2D(file);
            if(textureMapMode == aiTextureMapMode_Clamp) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            else if(textureMapMode == aiTextureMapMode_Wrap) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if(textureMapMode == aiTextureMapMode_Mirror) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
            }
            auto texture = textureBuilder.Build();
            builder.SetNormalTexture(texture);
        }

        if(pMaterial->GetTexture(aiTextureType_HEIGHT, 0, &path, nullptr, nullptr, nullptr, nullptr, &textureMapMode) == aiReturn_SUCCESS) {
            filesystem::path file = textureDir/filesystem::path(path.C_Str()).filename();
            auto textureBuilder = TextureBuilder().SetTexture2D(file);
            if(textureMapMode == aiTextureMapMode_Clamp) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            else if(textureMapMode == aiTextureMapMode_Wrap) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
            else if(textureMapMode == aiTextureMapMode_Mirror) {
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
                textureBuilder.SetParameter(GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
            }
            auto texture = textureBuilder.Build();
            builder.SetHeightTexture(texture);
        }

        return builder.Build();
    }

    static void LoadMesh(Model* model, const aiScene *pScene, const aiMesh *pMesh) {
        MeshBuilder builder;
        if(pMesh->HasPositions()) {
            std::vector<float> positions(pMesh->mNumVertices*3);
            memcpy(positions.data(), pMesh->mVertices, sizeof(float)*positions.size());
            builder.SetPositions(positions);
        }
        if(pMesh->HasNormals()) {
            std::vector<float> normals(pMesh->mNumVertices*3);
            memcpy(normals.data(), pMesh->mNormals, sizeof(float)*normals.size());
            builder.SetNormals(normals);
        }
        if(pMesh->HasTextureCoords(0)) {
            std::vector<float> uvs(pMesh->mNumVertices*2);
            for(size_t i = 0; i < pMesh->mNumVertices; i++) {
                uvs[i*2] = pMesh->mTextureCoords[0][i].x;
                uvs[i*2 + 1] = pMesh->mTextureCoords[0][i].y;
            }
            builder.SetTexCoords(uvs);
        }
        if(pMesh->HasBones()) {
            std::vector<int> joints(pMesh->mNumVertices*4, 0);
            std::vector<float> weights(pMesh->mNumVertices*4, 0.0f);
            for(size_t i = 0; i < pMesh->mNumBones; i++) {
                auto pBone = pMesh->mBones[i];
                auto pBoneNode = pBone->mNode;
                auto bone = model->CreateBone(pBone->mName.C_Str());
                aiMatrix4x4 offsetMatrix = pBone->mOffsetMatrix;
                bone->offsetMatrix = glm::make_mat4(&offsetMatrix.Transpose().a1);
                model->FindNode(bone->name)->bone = bone;

                for(size_t j = 0; j < pBone->mNumWeights; j++) {
                    for(size_t k = 0; k < 4; k++) {
                        if(weights[pBone->mWeights[j].mVertexId*4+k] == 0.0f) {
                            joints[pBone->mWeights[j].mVertexId*4+k] = bone->index;
                            weights[pBone->mWeights[j].mVertexId*4+k] = pBone->mWeights[j].mWeight;
                            break;
                        }
                    }
                }
            }
            builder.SetJoints(joints);
            builder.SetWeights(weights);
        }
        if(pMesh->HasTangentsAndBitangents()) {
            std::vector<float> tangents(pMesh->mNumVertices*3);
            memcpy(tangents.data(), pMesh->mTangents, sizeof(float)*tangents.size());
            builder.SetTangents(tangents);
        }
        if(pMesh->HasFaces()) {
            std::vector<uint32_t> indices(pMesh->mNumFaces*3);
            for(size_t i = 0; i < pMesh->mNumFaces; i++) {
                indices[i*3  ] = pMesh->mFaces[i].mIndices[0];
                indices[i*3+1] = pMesh->mFaces[i].mIndices[1];
                indices[i*3+2] = pMesh->mFaces[i].mIndices[2];
            }
            builder.SetIndices(indices);
        }

        builder.SetMaterial(LoadMaterial(model, pScene->mMaterials[pMesh->mMaterialIndex]));

        auto mesh = new Mesh(&builder);
        mesh->name = pMesh->mName.C_Str();
        model->AddMesh(mesh);
    }

    static void LoadAnimation(Animation* animation, const aiAnimation* pAnimation) {
        animation->SetName(pAnimation->mName.C_Str());
        animation->SetDuration((float)pAnimation->mDuration);
        if(pAnimation->mTicksPerSecond > 0) {
            animation->SetTicksPerSecond((float)pAnimation->mTicksPerSecond);
        }

        for(size_t i = 0; i < pAnimation->mNumChannels; i++) {
            auto pChannel = pAnimation->mChannels[i];
            AnimationChannel channel;
            channel.translationKeys.resize(pChannel->mNumPositionKeys);
            for(size_t j = 0; j < pChannel->mNumPositionKeys; j++) {
                channel.translationKeys[j].value = glm::make_vec3(&pChannel->mPositionKeys[j].mValue.x);
                channel.translationKeys[j].time = (float)pChannel->mPositionKeys[j].mTime;
            }
            channel.rotationKeys.resize(pChannel->mNumRotationKeys);
            for(size_t j = 0; j < pChannel->mNumRotationKeys; j++) {
                auto q = pChannel->mRotationKeys[j].mValue;
                channel.rotationKeys[j].value = glm::quat(q.w, q.x, q.y, q.z);
                channel.rotationKeys[j].time = (float)pChannel->mRotationKeys[j].mTime;
            }
            channel.scaleKeys.resize(pChannel->mNumScalingKeys);
            for(size_t j = 0; j < pChannel->mNumScalingKeys; j++) {
                channel.scaleKeys[j].value = glm::make_vec3(&pChannel->mScalingKeys[j].mValue.x);
                channel.scaleKeys[j].time = (float)pChannel->mScalingKeys[j].mTime;
            }
            animation->AddChannel(pChannel->mNodeName.C_Str(), channel);
        }
    }
};


#endif //ENGINE3D_SRC_IMPORTER_HPP
