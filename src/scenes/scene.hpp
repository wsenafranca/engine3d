//
// Created by Washington on 10/03/2020.
//

#ifndef ENGINE3D_SRC_SCENES_SCENE_HPP
#define ENGINE3D_SRC_SCENES_SCENE_HPP

#include <memory>
#include <vector>
#include <math/transform.hpp>
#include <graphics/node.hpp>
#include <components/light.hpp>
#include <components/camera.hpp>
#include <graphics/mesh.hpp>
#include <graphics/meshbatch.hpp>

class Scene {
public:
    Scene();
    virtual void Update(float dt);
    virtual void Render(MeshBatch *batch);

    std::shared_ptr<Node> CreateNode(const std::string &name);
    std::shared_ptr<Node> CreateLight(const std::string &name, const std::shared_ptr<Light> &light);
    std::shared_ptr<Node> CreateDirectionalLight(const std::string &name, const glm::vec3 &color, const glm::quat &direction);
    std::shared_ptr<Node> CreatePointLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &position);
    std::shared_ptr<Node> CreateSpotLight(const std::string &name, const glm::vec3 &color, const glm::quat &direction, const glm::vec3 &position, float innerAngle, float outerAngle);
    std::shared_ptr<Node> CreateMesh(const std::string &name, const std::shared_ptr<Mesh> &mesh);
    std::shared_ptr<Node> CreateCamera(const std::string &name, const std::shared_ptr<Camera> &camera);

    [[nodiscard]] const std::shared_ptr<Node> &GetRoot() const;
    [[nodiscard]] const std::weak_ptr<Light> &GetLight(uint64_t index) const;
    [[nodiscard]] const std::weak_ptr<Camera> &GetCamera(uint64_t index) const;

    uint64_t AddLight(const std::weak_ptr<Light> &light);
    uint64_t AddCamera(const std::weak_ptr<Camera> &camera);

    bool RemoveLight(const std::weak_ptr<Light> &light);
    bool RemoveCamera(const std::weak_ptr<Camera> &camera);

    [[nodiscard]] const std::weak_ptr<Camera> &GetActiveCamera() const;
    void SetActiveCamera(const std::weak_ptr<Camera> &activeCamera);

    [[nodiscard]] const std::vector< std::weak_ptr<Light> > &GetLights() const;
    [[nodiscard]] const std::vector< std::weak_ptr<Camera> > &GetCameras() const;

private:
    std::shared_ptr<Node> mRoot;
    std::weak_ptr<Camera> pActiveCamera;
    std::vector< std::weak_ptr<Light> > mLights;
    std::vector< std::weak_ptr<Camera> > mCameras;
};


#endif //ENGINE3D_SRC_SCENES_SCENE_HPP
