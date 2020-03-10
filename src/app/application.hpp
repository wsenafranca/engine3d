//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_APPLICATION_HPP
#define ENGINE3D_SRC_APPLICATION_HPP

#include <graphics/opengl.hpp>
#include <sys/messagehandler.hpp>
#include <sys/input.hpp>
#include "appinfo.hpp"
#include <sys/log.hpp>
#include <lua/luamanager.hpp>

class Application : public MessageHandler {
public:
    Application();
    int Exec(int argc, char** argv);
    void Exit();

    AppInfo *GetAppInfo() const;
    GLFWwindow *GetPWindow() const;

    static Application* sThisApplication;
protected:
    virtual void OnCreate();
    virtual void OnTick(float dt);
    virtual void OnDestroy();
protected:
    GLFWwindow *pWindow;
    std::unique_ptr<AppInfo> mAppInfo;
};


#endif //ENGINE3D_SRC_APPLICATION_HPP
