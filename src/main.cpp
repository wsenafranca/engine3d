#include "app/application.hpp"
#include "graphics/shaderprogram.hpp"
#include "graphics/meshbatch.hpp"
#include "graphics/model.hpp"
#include "graphics/meshbuilder.hpp"
#include "graphics/texturebuilder.hpp"
#include <controllers/charactercontroller.hpp>
#include <controllers/animcontroller.hpp>
#include <graphics/modelinstance.hpp>

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

    void SetCamera(const Camera *camera) override {
        mProgram->SetUniform("uView", camera->GetViewMatrix());
        mProgram->SetUniform("uProjection", camera->GetProjectionMatrix());
        mProgram->SetUniform("uCameraPosition", camera->worldPosition);
    }

    void SetLights(const std::list<Light *> &lights) override {
        int i = 0;
        for(const auto& light : lights) {
            mProgram->SetUniform("uLights[" + std::to_string(i) + "]", *light);
            i++;
        }
        mProgram->SetUniform("uNumLights", i);
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

    void SetCamera(const Camera *camera) override {
        mProgram->SetUniform("uView", glm::mat4(glm::mat3(camera->GetViewMatrix())));
        mProgram->SetUniform("uProjection", camera->GetProjectionMatrix());
    }

    void SetLights(const std::list<Light *> &lights) override {}

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

        mModel = new Model("data/models/xbot.fbx");
        mPlayer = mModel->CreateInstance();
        mAnimController = std::make_shared<AnimController>(mPlayer);
        auto idleState = mAnimController->AddAnimation("idle", new Animation("data/animations/xbot_idle.fbx"));
        idleState->playMode = ANIM_PLAY_MODE_LOOP;
        idleState->transitions = {
            {"moveForward",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return graph->GetProperty<bool>("moveForward").value_or(false);
                }
            },
            {"moveRightStrafe",
                    [](ANIM_STATE_TRANSITION_FUNC) {
                        return graph->GetProperty<bool>("moveRightStrafe").value_or(false);
                    }
            },
            {"moveLeftStrafe",
                    [](ANIM_STATE_TRANSITION_FUNC) {
                        return graph->GetProperty<bool>("moveLeftStrafe").value_or(false);
                    }
            },
            {"moveBackward",
                    [](ANIM_STATE_TRANSITION_FUNC) {
                        return graph->GetProperty<bool>("moveBackward").value_or(false);
                    }
            },
            {"jumpUp",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return graph->GetProperty<bool>("jumping").value_or(false);
                }
            }
        };
        auto moveForwardState = mAnimController->AddAnimation("moveForward", new Animation("data/animations/xbot_running.fbx"));
        moveForwardState->playMode = ANIM_PLAY_MODE_LOOP;
        moveForwardState->transitions = {
            {"idle",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return !graph->GetProperty<bool>("moveForward").value_or(false);
                }
            },
            {"runningJumpUp",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return graph->GetProperty<bool>("jumping").value_or(false);
                }
            }
        };
        auto moveRightStrafeState = mAnimController->AddAnimation("moveRightStrafe", new Animation("data/animations/xbot_right_strafe.fbx"));
        moveRightStrafeState->playMode = ANIM_PLAY_MODE_LOOP;
        moveRightStrafeState->transitions = {
                {"idle",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return !graph->GetProperty<bool>("moveRightStrafe").value_or(false);
                        }
                },
                {"runningJumpUp",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return graph->GetProperty<bool>("jumping").value_or(false);
                        }
                }
        };
        auto moveLeftStrafeState = mAnimController->AddAnimation("moveLeftStrafe", new Animation("data/animations/xbot_left_strafe.fbx"));
        moveLeftStrafeState->playMode = ANIM_PLAY_MODE_LOOP;
        moveLeftStrafeState->transitions = {
                {"idle",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return !graph->GetProperty<bool>("moveLeftStrafe").value_or(false);
                        }
                },
                {"runningJumpUp",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return graph->GetProperty<bool>("jumping").value_or(false);
                        }
                }
        };
        auto moveBackwardState = mAnimController->AddAnimation("moveBackward", new Animation("data/animations/xbot_running_backward.fbx"));
        moveBackwardState->playMode = ANIM_PLAY_MODE_LOOP;
        moveBackwardState->transitions = {
                {"idle",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return !graph->GetProperty<bool>("moveBackward").value_or(false);
                        }
                },
                {"runningJumpUp",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return graph->GetProperty<bool>("jumping").value_or(false);
                        }
                }
        };
        auto jumpUpState = mAnimController->AddAnimation("jumpUp", new Animation("data/animations/xbot_jump_up.fbx"));
        jumpUpState->playMode = ANIM_PLAY_MODE_NORMAL;
        jumpUpState->transitions = {
            {"jumpDown",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return frame > 3 && (graph->GetProperty<bool>("falling").value_or(false));
                }
            }
        };
        jumpUpState->frameCallback[2] = [this]() {
            this->mPlayerController.Jump(6.0f);
        };
        jumpUpState->startCallback = [this]() {
            this->mPlayerController.SetMoveEnabled(false);
        };
        auto jumpDownState = mAnimController->AddAnimation("jumpDown", new Animation("data/animations/xbot_jump_down.fbx"));
        jumpDownState->playMode = ANIM_PLAY_MODE_NORMAL;
        jumpDownState->transitions = {
            {"landing",
                [](ANIM_STATE_TRANSITION_FUNC) {
                    return graph->GetProperty<bool>("grounded").value_or(false);
                }
            }
        };
        auto landingState = mAnimController->AddAnimation("landing", new Animation("data/animations/xbot_landing.fbx"));
        landingState->playMode = ANIM_PLAY_MODE_NORMAL;
        landingState->transitions = {
                {"idle",
                            [](ANIM_STATE_TRANSITION_FUNC) {
                                return state->isFinished && !graph->GetProperty<bool>("isMoving").value_or(false);
                            }
                },
                {"moveForward",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return state->isFinished && graph->GetProperty<bool>("isMoving").value_or(false);
                        }
                }
        };
        landingState->endCallback = [this]() {
            this->mPlayerController.SetMoveEnabled(true);
        };
        auto runningJumpUpState = mAnimController->AddAnimation("runningJumpUp", new Animation("data/animations/xbot_jump_up.fbx"));
        runningJumpUpState->playMode = ANIM_PLAY_MODE_NORMAL;
        runningJumpUpState->transitions = {
                {"runningJumpDown",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return frame > 3 && (graph->GetProperty<bool>("falling").value_or(false));
                        }
                }
        };
        runningJumpUpState->frameCallback[2] = [this]() {
            this->mPlayerController.Jump(6.0f);
        };
        runningJumpUpState->startCallback = [this]() {
            this->mPlayerController.SetMoveEnabled(false);
        };
        auto runningJumpDownState = mAnimController->AddAnimation("runningJumpDown", new Animation("data/animations/xbot_jump_down.fbx"));
        runningJumpDownState->playMode = ANIM_PLAY_MODE_NORMAL;
        runningJumpDownState->transitions = {
                {"landing",
                        [](ANIM_STATE_TRANSITION_FUNC) {
                            return graph->GetProperty<bool>("grounded").value_or(false);
                        }
                }
        };
        runningJumpDownState->endCallback = [this]() {
            this->mPlayerController.SetMoveEnabled(true);
        };

        mCamera = new Camera(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        mCamera->restPosition = glm::vec3(0.0f, 5.0f, -8.0f);
        mCamera->position = mCamera->restPosition;
        mCamera->LookAt(glm::vec3(0.0f, 1.0f, 0.0f));

        mPlayer->GetModel()->GetRootNode()->camera.reset(mCamera);

        mLightSystem.CreateDirectionalLight("DirLight", glm::vec3(0.2f), glm::vec3(-0.2f, -1.0f, -0.3f));
        mLightSystem.CreatePointLight("PointLight", glm::vec3(0.7f, 0.1f, 0.0f), glm::vec3(4.0f, 4.0f, -4.0f));
        mLightSystem.CreatePointLight("PointLight1", glm::vec3(0.2f, 0.8f, 0.0f), glm::vec3(-4.0f, 4.0f, -4.0f));
        mLightSystem.CreatePointLight("PointLight2", glm::vec3(0.3f, 0.0f, 9.0f), glm::vec3(0.0f, 4.0f, 4.0f));
        mSpotLight = mLightSystem.CreateSpotLight("SpotLight",
                glm::vec3(1.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, -2.5f, -10.0f),
                glm::vec3(0.0f, 5.0f, 0.0f),
                glm::radians(30.0f),
                glm::radians(60.0f));
        mBatch.SetLightSystem(&mLightSystem);

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

        mSkyBox = MeshBuilder()
                .SetBox(glm::vec3(1.0f))
                .SetMaterial(MaterialBuilder()
                    .SetDiffuseTexture(TextureBuilder().SetSkyBox("data/textures/skybox", "jpg").Build())
                    .Build())
                .Build();

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    }

    void OnTick(float dt) override {
        static float stateTime = 0.0f;
        static bool startJumping = false;
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
        stateTime += dt;

        if(Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            auto rot = glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_X), glm::vec3(0.0f, 1.0f, 0.0f))*
                         glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_Y), glm::vec3(1.0f, 0.0f, 0.0f));
            mCamera->position = rot*mCamera->position;
        } else {
            if(mPlayerController.IsMoving()) {
                mCamera->position = mCamera->restPosition;
            }
            if(Input::GetMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                mPlayerController.Yaw(Input::GetAxis(INPUT_AXIS_MOUSE_X));
                mCamera->position = glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_Y)*0.2f, glm::vec3(1.0f, 0.0f, 0.0f))*mCamera->position;
                //mCamera->yaw = mPlayerController.mYaw;
                //mCamera->position = glm::angleAxis(Input::GetAxis(INPUT_AXIS_MOUSE_Y), glm::vec3(1.0f, 0.0f, 0.0f))*mCamera->position;
                mCamera->restPosition = mCamera->position;
            }
        }
        //mCamera->LookAt(mPlayerController.GetPosition());
        //mCamera->transform.rotation = glm::quatLookAt(-glm::normalize(mCamera->transform.position), glm::vec3(0.0f, 1.0f, 0.0f));

        if(mPlayerController.IsGrounded() && !startJumping) {
            auto yaw = mPlayerController.mYaw;
            auto rot = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            auto moveForward = rot*glm::vec3(0.0f, 0.0f, -1.0f);
            auto moveRight = rot*glm::vec3(-1.0f, 0.0f, 0.0f);

            auto move = moveForward*Input::GetAxis(INPUT_AXIS_Y) + moveRight*Input::GetAxis(INPUT_AXIS_X);
            if(glm::length(move) > 1) {
                move = glm::normalize(move);
            }
            move*=6.0f;
            if(move.z < 0) {
                move.z *= 0.5f;
            }
            mPlayerController.Move(move);

            if(Input::GetKeyDown(KEY_SPACE) && !startJumping) {
                startJumping = true;
            }
        }
        if(mPlayerController.IsJumping()) {
            startJumping = false;
        }

        float fov = mCamera->GetFieldOfView();
        fov = glm::clamp(fov - Input::GetAxis(INPUT_AXIS_SCROLL_WHEEL),
                         glm::radians(5.0f),
                         glm::radians(80.0f));
        mCamera->SetFieldOfView(fov);

        mPlayerController.Update(dt);

        mPlayer->SetPosition(mPlayerController.GetPosition());
        mPlayer->SetRotation(mPlayerController.GetRotation());


        mSpotLight->direction = mPlayerController.GetPosition() - mSpotLight->position;

        mAnimController->SetProperty("grounded", mPlayerController.IsGrounded());
        mAnimController->SetProperty("isMoving", mPlayerController.IsMoving());
        mAnimController->SetProperty("jumping", startJumping);
        mAnimController->SetProperty("falling", mPlayerController.IsFalling());
        mAnimController->SetProperty("moveForward", Input::GetAxis(INPUT_AXIS_Y) < 0);
        mAnimController->SetProperty("moveBackward", Input::GetAxis(INPUT_AXIS_Y) > 0);
        mAnimController->SetProperty("moveRightStrafe", Input::GetAxis(INPUT_AXIS_X) > 0);
        mAnimController->SetProperty("moveLeftStrafe", Input::GetAxis(INPUT_AXIS_X) < 0);

        mAnimController->Update(dt);

        mBatch.Begin(mCamera, mShader);
        mBatch.Render(mFloor.get(), glm::mat4(1.0f));
        mPlayer->Render(&mBatch);
        mBatch.End();

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);
        mBatch.Begin(mCamera, mSkyBoxShader);
        mBatch.Render(mSkyBox.get(), glm::mat4(1.0f));
        mBatch.End();
        glDepthFunc(GL_LESS);
        glCullFace(GL_BACK);
    }

    void OnDestroy() override {
        delete mShader;
        delete mSkyBoxShader;
        delete mCamera;
        //delete mModel;
    }

    MeshBatch mBatch;
    MyShader *mShader{nullptr};
    SkyBoxShader *mSkyBoxShader{nullptr};
    Camera *mCamera{nullptr};
    LightSystem mLightSystem;
    Light* mSpotLight{nullptr};
    Model *mModel{nullptr};
    std::shared_ptr<ModelInstance> mPlayer;
    CharacterController mPlayerController;
    std::shared_ptr<AnimController> mAnimController;
    std::shared_ptr<Mesh> mFloor;
    std::shared_ptr<Mesh> mSkyBox;
};

int main(int argc, char** argv) {
    MyApp app;
    return app.Exec(argc, argv);
}
