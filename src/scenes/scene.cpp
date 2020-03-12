//
// Created by Washington on 10/03/2020.
//

#include "scene.hpp"
#include <graphics/meshbatch.hpp>

Scene::Scene() : mRoot(new Node()) {
}

void Scene::Update(float dt) {
    mRoot->Update(dt);
}

void Scene::Render(MeshBatch *batch) {
    mRoot->Render(batch);
}

std::shared_ptr<Node> Scene::CreateNode(const std::string &name) {
    auto node = std::make_shared<Node>();
    node->SetName(name);
    return node;
}

std::shared_ptr<Node> Scene::CreateLight(const std::string &name, const std::shared_ptr<Light> &light) {
    auto node = CreateNode(name);
    AddLight(light);
    node->AddComponent(light);
    return node;
}

std::shared_ptr<Node>
Scene::CreateDirectionalLight(const std::string &name, const glm::vec3 &color, const glm::quat &direction) {
    auto light = std::make_shared<Light>(LIGHT_TYPE_DIRECTIONAL);
    light->SetColor(color);
    auto node = CreateLight(name, light);
    node->SetRotation(direction);
    return node;
}

std::shared_ptr<Node>
Scene::CreatePointLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &position) {
    auto light = std::make_shared<Light>(LIGHT_TYPE_POINT);
    light->SetColor(color);
    auto node = CreateLight(name, light);
    node->SetPosition(position);
    return node;
}

std::shared_ptr<Node>
Scene::CreateSpotLight(const std::string &name, const glm::vec3 &color, const glm::quat &direction,
                       const glm::vec3 &position, float innerAngle, float outerAngle) {
    auto light = std::make_shared<Light>(LIGHT_TYPE_SPOT);
    light->SetColor(color);
    light->SetSpotInnerAngle(innerAngle);
    light->SetSpotOuterAngle(outerAngle);
    auto node = CreateLight(name, light);
    node->SetPosition(position);
    return node;
}


std::shared_ptr<Node> Scene::CreateMesh(const std::string &name, const std::shared_ptr<Mesh> &mesh) {
    auto node = CreateNode(name);
    node->AddMesh(mesh);
    return node;
}

std::shared_ptr<Node> Scene::CreateCamera(const std::string &name, const std::shared_ptr<Camera> &camera) {
    auto node = CreateNode(name);
    AddCamera(camera);
    node->AddComponent(camera);
    return node;
}

const std::shared_ptr<Node> &Scene::GetRoot() const {
    return mRoot;
}

const std::weak_ptr<Light> &Scene::GetLight(uint64_t index) const {
    return mLights[index];
}

const std::weak_ptr<Camera> &Scene::GetCamera(uint64_t index) const {
    return mCameras[index];
}

uint64_t Scene::AddLight(const std::weak_ptr<Light> &light) {
    auto id = mLights.size();
    mLights.push_back(light);
    return id;
}

uint64_t Scene::AddCamera(const std::weak_ptr<Camera> &camera) {
    auto id = mCameras.size();
    mCameras.push_back(camera);
    if(!pActiveCamera.lock()) {
        pActiveCamera = camera;
    }
    return id;
}

bool Scene::RemoveLight(const std::weak_ptr<Light> &light) {
    auto lightLock = light.lock();
    if(!lightLock) return false;
    bool any = false;
    for(auto it = mLights.begin(); it != mLights.end();) {
        if(it->lock() == lightLock) {
            it = mLights.erase(it);
            any = true;
        } else {
            ++it;
        }
    }
    return any;
}

bool Scene::RemoveCamera(const std::weak_ptr<Camera> &camera) {
    auto cameraLock = camera.lock();
    if(!cameraLock) return false;
    bool any = false;
    for(auto it = mCameras.begin(); it != mCameras.end();) {
        if(it->lock() == cameraLock) {
            it = mCameras.erase(it);
            any = true;
        } else {
            ++it;
        }
    }
    return any;
}

const std::weak_ptr<Camera> &Scene::GetActiveCamera() const {
    return pActiveCamera;
}

void Scene::SetActiveCamera(const std::weak_ptr<Camera> &activeCamera) {
   pActiveCamera = activeCamera;
}

const std::vector< std::weak_ptr<Light> > &Scene::GetLights() const {
    return mLights;
}

const std::vector< std::weak_ptr<Camera> > &Scene::GetCameras() const {
    return mCameras;
}
