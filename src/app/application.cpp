//
// Created by Washington on 21/02/2020.
//

#include "application.hpp"
#include <memory>
#include <stdexcept>
#include "config.hpp"

Application* Application::sThisApplication = nullptr;

Application::Application() : pWindow(nullptr) {}

int Application::Exec(int argc, char** argv) {

    sThisApplication = this;
    Config config;
    try {
        config.LoadFile("Engine3D.ini");
    }catch (...) {}

    mAppInfo = std::make_unique<AppInfo>();
    mAppInfo->appName = argv[0];
    mAppInfo->windowPositionX = config.GetInt("windowPositionX").value_or(100);
    mAppInfo->windowPositionY = config.GetInt("windowPositionY").value_or(100);
    mAppInfo->windowWidth = config.GetInt("windowWidth").value_or(800);
    mAppInfo->windowHeight = config.GetInt("windowHeight").value_or(600);
    for(int i = 0; i < argc; i++) {
        mAppInfo->args.emplace_back(argv[i]);
    }

    glfwSetErrorCallback([](int error, const char* description) {
        throw std::runtime_error(std::string("Error(" + std::to_string(error) + "): ") + description);
    });

    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    pWindow = glfwCreateWindow(mAppInfo->windowWidth, mAppInfo->windowHeight, mAppInfo->appName.c_str(), nullptr, nullptr);
    glfwSetWindowPos(pWindow, mAppInfo->windowPositionX, mAppInfo->windowPositionY);

    if(!pWindow) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(pWindow);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        throw std::runtime_error((char*)glewGetErrorString(err));
    }

    glfwSetWindowUserPointer(pWindow, this);

    glfwSwapInterval(1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    Input::Connect();
    MessageManager::RegisterWindow(pWindow);

    OnCreate();

    //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double t0 = glfwGetTime();
    double t;
    float dt;
    while(!glfwWindowShouldClose(pWindow)) {
        t = glfwGetTime();
        dt = (float)(t - t0);
        t0 = t;
        glfwPollEvents();
        MessageManager::DispatchMessages();
        Input::Update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        OnTick(dt);
        glfwSwapBuffers(pWindow);
    }

    OnDestroy();
    Input::Disconnect();

    config.Put("windowPositionX", mAppInfo->windowPositionX);
    config.Put("windowPositionY", mAppInfo->windowPositionY);
    config.Put("windowWidth", (int) mAppInfo->windowWidth);
    config.Put("windowHeight", (int) mAppInfo->windowHeight);
    config.SaveFile("Engine3D.ini");

    glfwDestroyWindow(pWindow);
    glfwTerminate();
    sThisApplication = nullptr;
    return 0;
}

void Application::OnCreate() {

}

void Application::OnTick(float) {

}

void Application::OnDestroy() {

}

void Application::Exit() {
    glfwSetWindowShouldClose(pWindow, true);
}

AppInfo *Application::GetAppInfo() const {
    return mAppInfo.get();
}

GLFWwindow *Application::GetPWindow() const {
    return pWindow;
}
