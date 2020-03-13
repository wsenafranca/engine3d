#include <app/application.hpp>
#include <graphics/shaderprogram.hpp>
#include <graphics/shader.hpp>
#include <graphics/meshbatch.hpp>
#include <graphics/meshbuilder.hpp>
#include <graphics/texturebuilder.hpp>
#include <components/camera.hpp>
#include <components/light.hpp>
#include <components/animator.hpp>
#include <scenes/scene.hpp>
#include <controllers/playercontroller.hpp>
#include <controllers/maincameracontroller.hpp>
#include <physics/world.hpp>
#include <components/rigidbody.hpp>

class MyShader : public Shader {
public:
    MyShader() {
        mProgram = new ShaderProgram("mesh.vert", "mesh.frag");
        mProgram->Compile();
    }
    ~MyShader() override {
        delete mProgram;
    }

    [[nodiscard]] ShaderProgram *GetProgram() const override {
        return mProgram;
    }

    void SetCamera(const Camera &camera) override {
        mProgram->SetUniform("uView", camera.GetViewMatrix());
        mProgram->SetUniform("uProjection", camera.GetProjectionMatrix());
        mProgram->SetUniform("uCameraPosition", camera.GetWorldPosition());
    }

    void SetLights(const std::vector< std::weak_ptr<Light> >&lights) override {
        int numLights = 0;
        for(const auto & light : lights) {
            auto lock = light.lock();
            if(lock) {
                mProgram->SetUniform("uLights[" + std::to_string(numLights) + "]", *(lock));
                numLights++;
            }
        }
        mProgram->SetUniform("uNumLights", numLights);
    }

    void SetWorldMatrix(const glm::mat4 &worldMatrix) override {
        mProgram->SetUniform("uModel", worldMatrix);
    }

    void SetMaterial(const Material &material) override {
        mProgram->SetUniform("uMaterial", material);
    }

    void SetAttributeBits(uint32_t attributeBits) override {
        mProgram->SetUniform("uAttributeBits", attributeBits);
    }

    void SetBoneMatrices(const std::vector<glm::mat4> &matrices) override {
        for(size_t i = 0; i < matrices.size(); i++) {
            mProgram->SetUniform("uBoneMatrix["+ std::to_string(i) +"]", matrices[i]);
        }
    }

    ShaderProgram* mProgram{nullptr};
};

class SkyBoxShader : public Shader {
public:
    SkyBoxShader() {
        mProgram = new ShaderProgram("skybox.vert", "skybox.frag");
        mProgram->Compile();
    }
    ~SkyBoxShader() override {
        delete mProgram;
    }

    [[nodiscard]] ShaderProgram *GetProgram() const override {
        return mProgram;
    }

    void SetCamera(const Camera &camera) override {
        mProgram->SetUniform("uView", glm::mat3(camera.GetViewMatrix()));
        mProgram->SetUniform("uProjection", camera.GetProjectionMatrix());
    }

    void SetLights(const std::vector< std::weak_ptr<Light> >&lights) override {}

    void SetWorldMatrix(const glm::mat4 &worldMatrix) override {}

    void SetMaterial(const Material &material) override {
        if(material.diffuseTexture) {
            mProgram->SetUniform("uSkyBox", 0);
            material.diffuseTexture->BindActive(0);
        }
    }

    void SetAttributeBits(uint32_t attributeBits) override {}

    void SetBoneMatrices(const std::vector<glm::mat4> &matrices) override {}

    ShaderProgram* mProgram{nullptr};
};

class MyApp : public Application {
protected:
    void OnCreate() override {
        mShader = new MyShader();
        mSkyBoxShader = new SkyBoxShader();

        mWorld = new World(glm::vec3(0.0f, 10.0f, 0.0f));

        mScene = new Scene();
        mPlayer = Node::Load("data/models/xbot.fbx", mScene);
        mPlayer->AddComponent(Animator::Load("player"));
        mPlayer->AddComponent(
                std::make_shared<RigidBody>(mWorld->CreateBody(glm::vec3(0.0f), glm::vec3(1.0f))));

        mPlayer->SetController(std::make_shared<PlayerController>());
        mScene->GetRoot()->AddNode(mPlayer);

        auto camera = std::make_shared<Camera>(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        mCamera = mScene->CreateCamera("MainCamera", camera);
        mCamera->SetPosition(glm::vec3(0.0f, 5.0f, 8.0f));
        mCamera->SetRotation(glm::angleAxis(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        auto camController = std::make_shared<MainCameraController>();
        camController->SetCameraTarget(mPlayer);
        mCamera->SetController(camController);
        mScene->GetRoot()->AddNode(mCamera);

        mScene->GetRoot()->AddNode(mScene->CreateDirectionalLight("DirLight", glm::vec3(0.2f), glm::quatLookAt(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f, 1.0f, 0.0f))));
        mScene->GetRoot()->AddNode(mScene->CreatePointLight("PointLight", glm::vec3(0.7f, 0.1f, 0.0f), glm::vec3(4.0f, 4.0f, -4.0f)));
        mScene->GetRoot()->AddNode(mScene->CreatePointLight("PointLight1", glm::vec3(0.2f, 0.8f, 0.0f), glm::vec3(-4.0f, 4.0f, -4.0f)));
        mScene->GetRoot()->AddNode(mScene->CreatePointLight("PointLight2", glm::vec3(0.3f, 0.0f, 9.0f), glm::vec3(0.0f, 4.0f, 4.0f)));
        mSpotLight = mScene->CreateSpotLight("SpotLight",
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::quatLookAt(glm::vec3(0.0f, -2.5f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                glm::vec3(0.0f, 5.0f, 0.0f),
                glm::radians(30.0f),
                glm::radians(60.0f));
        mScene->GetRoot()->AddNode(mSpotLight);

        mFloor = MeshBuilder()
                .SetPlane(
                        glm::vec3(-100.0f, 0.0f,  100.0f),
                        glm::vec3( 100.0f, 0.0f,  100.0f),
                        glm::vec3( 100.0f, 0.0f, -100.0f),
                        glm::vec3(-100.0f, 0.0f, -100.0f))
                .SetTexCoords({
                    0.0f, 0.0f,
                    100.0f, 0.0f,
                    100.0f, 100.0f,
                    0.0f, 100.0f})
                .SetMaterial(MaterialBuilder()
                    .SetDiffuseTexture(TextureBuilder()
                        .SetTexture2D("data/textures/Wood_Pattern_001_basecolor.jpg")
                        .Build())
                    .SetOcclusionTexture(TextureBuilder()
                        .SetTexture2D("data/textures/Wood_Pattern_001_ambientOcclusion.jpg")
                        .Build())
                    .SetNormalTexture(TextureBuilder()
                        .SetTexture2D("data/textures/Wood_Pattern_001_normal.jpg")
                        .Build())
                    .SetHeightTexture(TextureBuilder()
                        .SetTexture2D("data/textures/Wood_Pattern_001_height.png")
                        .Build())
                    .Build())
                .Build();
        mWorld->CreateBody(glm::vec3(0.0f), glm::vec3(100.0f, 0.1f, 100.0f));

        mSkyBox = MeshBuilder()
                .SetBox(glm::vec3(1.0f))
                .SetMaterial(MaterialBuilder()
                    .SetDiffuseTexture(TextureBuilder().SetSkyBox("data/textures/skybox", "jpg").Build())
                    .Build())
                .Build();

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    }

    void OnTick(float dt) override {
        static float timeScale = 1.0f;

        if(Input::GetKeyDown(KEY_F1)) {
            timeScale = 1.0f;
        } else if(Input::GetKeyDown(KEY_F2)) {
            timeScale = 0.1f;
        } else if(Input::GetKeyDown(KEY_F3)) {
            timeScale = 2.0f;
        } else if(Input::GetKeyDown(KEY_ESCAPE)) {
            Exit();
        }

        dt*=timeScale;

        mWorld->Update(dt);

        mScene->Update(dt);
        mBatch.Begin(mScene, mShader);
        mBatch.Render(mFloor.get(), glm::mat4(1.0f));
        mScene->Render(&mBatch);
        mBatch.End();

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);
        mBatch.Begin(mScene, mSkyBoxShader);
        mBatch.Render(mSkyBox.get(), glm::mat4(1.0f));
        mBatch.End();
        glDepthFunc(GL_LESS);
        glCullFace(GL_BACK);
    }

    void OnDestroy() override {
        delete mShader;
        delete mSkyBoxShader;
        delete mScene;
        delete mWorld;
    }

    MeshBatch mBatch;
    MyShader *mShader{nullptr};
    SkyBoxShader *mSkyBoxShader{nullptr};
    std::shared_ptr<Node> mCamera;
    std::shared_ptr<Node> mSpotLight{nullptr};
    Scene* mScene{nullptr};
    std::shared_ptr<Node> mPlayer;
    std::shared_ptr<Mesh> mFloor;
    std::shared_ptr<Mesh> mSkyBox;
    World* mWorld{nullptr};
};

int main(int argc, char** argv) {
    MyApp app;
    return app.Exec(argc, argv);
}
