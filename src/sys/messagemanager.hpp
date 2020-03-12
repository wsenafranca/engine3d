//
// Created by Washington on 23/02/2020.
//

#ifndef ENGINE3D_SRC_MESSAGEMANAGER_HPP
#define ENGINE3D_SRC_MESSAGEMANAGER_HPP

#include <future>
#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <list>
#include "message.hpp"
#include "../base/singleton.hpp"
#include <graphics/opengl.hpp>

class MessageHandler;

class TMessageManager {
public:
    void AddHandler(int event, MessageHandler* handler);
    void RemoveHandler(int event, MessageHandler* handler);
    void RemoveHandler(MessageHandler* handler);

    void SendMessageEvent(int event, void* data);
    void DispatchMessages();

    // GLFW event callback
    static void RegisterWindow(GLFWwindow* window);
    static void WindowPosCallback(GLFWwindow* window, int x, int y);
    static void WindowSizeCallback(GLFWwindow* window, int x, int y);
    static void WindowMaximizeCallback(GLFWwindow* window, int maximized);
    static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);
    static void WindowCloseCallback(GLFWwindow* window);
    static void WindowRefreshCallback(GLFWwindow* window);
    static void WindowFocusCallback(GLFWwindow* window, int focused);
    static void WindowIconifyCallback(GLFWwindow* window, int iconified);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double x, double y);
    static void CursorEnterCallback(GLFWwindow* window, int entered);
    static void ScrollCallback(GLFWwindow* window, double x, double y);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MonitorCallback(GLFWmonitor* monitor, int action);
    static void JoystickCallback(int jid, int action);

private:
    std::unordered_map<int, std::list<MessageHandler*> > mHandlerMap;
    std::mutex mMessageQueueMutex;
    std::queue<Message> mMessageQueue;
};

namespace MessageManager {
    inline void RegisterWindow(GLFWwindow* window) {TMessageManager::RegisterWindow(window);}
    inline void AddHandler(int event, MessageHandler* handler) {Singleton<TMessageManager>::GetInstance()->AddHandler(event, handler);}
    inline void RemoveHandler(int event, MessageHandler* handler) {Singleton<TMessageManager>::GetInstance()->RemoveHandler(event, handler);}
    inline void RemoveHandler(MessageHandler* handler) {Singleton<TMessageManager>::GetInstance()->RemoveHandler(handler);}
    inline void SendMessage(int event, void* data) {Singleton<TMessageManager>::GetInstance()->SendMessageEvent(event, data);}
    inline void DispatchMessages() {Singleton<TMessageManager>::GetInstance()->DispatchMessages();}
}

#endif //ENGINE3D_SRC_MESSAGEMANAGER_HPP
