//
// Created by Washington on 24/02/2020.
//

#ifndef ENGINE3D_SRC_IMPORTER_HPP
#define ENGINE3D_SRC_IMPORTER_HPP

#include "../graphics/node.hpp"
#include "../graphics/meshbuilder.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include "../graphics/texturebuilder.hpp"
#include "../graphics/opengl.hpp"
#include "animations/animation.hpp"
#include <scenes/scene.hpp>

class Importer {
public:
    std::shared_ptr<Node> LoadNode(const filesystem::path& file, Scene *scene) {
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

        meshes.resize(pScene->mNumMeshes);
        for(size_t i = 0; i < pScene->mNumMeshes; i++) {
            LoadMesh(pScene, i);
        }

        std::string rootName = file.filename().substr(0, file.filename().rfind('.'));
        auto root = scene->CreateNode(rootName);
        root->AddNode(BuildNode(scene, pScene->mRootNode));

        for(auto &it : skeletons) {
            it.second->root = root;
            BuildSkeleton(it.second.get(), nodes[it.first]);
        }

        importer.FreeScene();

        return root;
    }

    std::shared_ptr<Animation> LoadAnimation(const filesystem::path& file) {
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

        auto animation = std::make_shared<Animation>();
        if(pScene->mNumAnimations > 0) {
            auto pAnimation = pScene->mAnimations[0];
            LoadAnimation(animation, pAnimation);
        }

        importer.FreeScene();

        return animation;
    }

protected:
    std::shared_ptr<Node> BuildNode(Scene *scene, const aiNode *pNode) {
        auto node = scene->CreateNode(pNode->mName.C_Str());
        aiMatrix4x4 transformation = pNode->mTransformation;
        aiVector3D t, s;
        aiQuaternion r;
        transformation.Decompose(s, r, t);
        node->SetLocalTransform(Transform(
                glm::vec3(t.x, t.y, t.z),
                glm::quat(r.w, r.x, r.y, r.z),
                glm::vec3(s.x, s.y, s.z)));

        for(size_t i = 0; i < pNode->mNumMeshes; i++) {
            node->AddMesh(meshes[pNode->mMeshes[i]]);
        }

        auto it = bones.find(node->GetName());
        if(it != bones.end()) {
            node->SetBone(it->second);
        }

        nodes[node->GetName()] = node;
        for(size_t i = 0; i < pNode->mNumChildren; i++) {
            node->AddNode(BuildNode(scene, pNode->mChildren[i]));
        }

        return node;
    }

    static std::shared_ptr<Material> LoadMaterial(const aiMaterial *pMaterial) {
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

    void LoadMesh(const aiScene *pScene, uint32_t meshIndex) {
        auto pMesh = pScene->mMeshes[meshIndex];
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
            std::shared_ptr<Skeleton> skeleton;
            for(size_t i = 0; i < pMesh->mNumBones; i++) {
                auto pBone = pMesh->mBones[i];
                auto pBoneNode = pBone->mNode;

                std::string boneName = pBone->mName.C_Str();
                std::shared_ptr<Bone> bone;
                auto it = bones.find(boneName);
                if(it != bones.end()) {
                    bone = it->second;
                } else {
                    aiMatrix4x4 offsetMatrix = pBone->mOffsetMatrix;
                    bone = std::make_shared<Bone>();
                    bone->name = boneName;
                    bone->index = boneMap.size();
                    bone->offsetMatrix = glm::make_mat4(&offsetMatrix.Transpose().a1);

                    std::string skeletonName = pBone->mArmature->mName.C_Str();
                    auto skeletonIt = skeletons.find(skeletonName);
                    if(skeletonIt != skeletons.end()) {
                        skeleton = skeletonIt->second;
                    } else {
                        skeleton = std::make_shared<Skeleton>();
                        skeleton->name = skeletonName;
                        skeletons[skeletonName] = skeleton;
                    }
                    boneMap[boneName] = bone->index;
                    bones[boneName] = bone;
                }

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
            builder.SetSkeleton(skeleton);
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

        builder.SetMaterial(LoadMaterial(pScene->mMaterials[pMesh->mMaterialIndex]));

        auto mesh = builder.Build();
        mesh->name = pMesh->mName.C_Str();
        meshes[meshIndex] = mesh;
    }

    static void BuildSkeleton(Skeleton* skeleton, const std::shared_ptr<Node> &node) {
        skeleton->joints.push_back(node);
        for(auto &child : node->GetChildren()) {
            BuildSkeleton(skeleton, child);
        }
    }

    static void LoadAnimation(std::shared_ptr<Animation> &animation, const aiAnimation* pAnimation) {
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

    std::unordered_map< std::string, std::shared_ptr<Node> > nodes;
    std::unordered_map< std::string, std::shared_ptr<Bone> > bones;
    std::unordered_map< std::string, uint32_t > boneMap;
    std::unordered_map< std::string, std::shared_ptr<Skeleton> > skeletons;
    std::vector< std::shared_ptr<Mesh> > meshes;
};


#endif //ENGINE3D_SRC_IMPORTER_HPP
