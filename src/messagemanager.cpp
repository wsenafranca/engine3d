//
// Created by Washington on 23/02/2020.
//

#include "messagemanager.hpp"
#include "messagehandler.hpp"
#include "opengl.hpp"
#include "application.hpp"

void TMessageManager::AddHandler(int event, MessageHandler *handler) {
    std::unique_lock<std::mutex> lock(mMessageQueueMutex);
    mHandlerMap[event].push_back(handler);
}

void TMessageManager::RemoveHandler(int event, MessageHandler *handler) {
    std::unique_lock<std::mutex> lock(mMessageQueueMutex);
    auto& handlers = mHandlerMap[event];
    for(auto it = handlers.begin(); it != handlers.end(); ++it) {
        if(*it == handler) {
            handlers.erase(it);
            return;
        }
    }
}

void TMessageManager::RemoveHandler(MessageHandler *handler) {
    std::unique_lock<std::mutex> lock(mMessageQueueMutex);
    for(auto& p : mHandlerMap) {
        for(auto it = p.second.begin(); it != p.second.end(); ++it) {
            if(*it == handler) {
                p.second.erase(it);
                break;
            }
        }
    }
}

void TMessageManager::SendMessageEvent(int event, void *data)  {
    std::unique_lock<std::mutex> lock(mMessageQueueMutex);
    mMessageQueue.push(Message{event, data});
}

void TMessageManager::DispatchMessages() {
    std::queue<Message> queue;
    {
        std::unique_lock<std::mutex> lock(mMessageQueueMutex);
        mMessageQueue.swap(queue);
    }

    while(!queue.empty()) {
        const auto& message = queue.front();
        for(auto it : mHandlerMap[message.event]) {
            it->OnMessageReceived(message.event, message.data);
        }
        if(message.data) free(message.data);
        queue.pop();
    }
}

void TMessageManager::RegisterWindow(GLFWwindow *window) {
    glfwSetMonitorCallback(MonitorCallback);
    glfwSetJoystickCallback(JoystickCallback);
    glfwSetWindowPosCallback(window, WindowPosCallback);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);
    glfwSetWindowCloseCallback(window, WindowCloseCallback);
    glfwSetWindowRefreshCallback(window, WindowRefreshCallback);
    glfwSetWindowFocusCallback(window, WindowFocusCallback);
    glfwSetWindowIconifyCallback(window, WindowIconifyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetCursorEnterCallback(window, CursorEnterCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowMaximizeCallback(window, WindowMaximizeCallback);
    glfwSetWindowContentScaleCallback(window, WindowContentScaleCallback);
}

void TMessageManager::WindowPosCallback(GLFWwindow*, int x, int y) {
    Application::sThisApplication->GetAppInfo()->windowPositionX = x;
    Application::sThisApplication->GetAppInfo()->windowPositionY = y;
    struct Data {
        int x, y;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->x = x;
    data->y = y;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_POSITION_CHANGED, data);
}

void TMessageManager::WindowSizeCallback(GLFWwindow*, int x, int y) {
    Application::sThisApplication->GetAppInfo()->windowWidth = x;
    Application::sThisApplication->GetAppInfo()->windowHeight = y;
    struct Data {
        int x, y;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->x = x;
    data->y = y;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_SIZE_CHANGED, data);
}

void TMessageManager::WindowMaximizeCallback(GLFWwindow*, int maximized) {
    struct Data {
        int maximized;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->maximized = maximized;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_SIZE_MAXIMIZED, data);
}

void TMessageManager::WindowContentScaleCallback(GLFWwindow*, float xscale, float yscale) {
    struct Data {
        float x, y;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->x = xscale;
    data->y = yscale;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_CONTENT_SCALE_CHANGED, data);
}

void TMessageManager::WindowCloseCallback(GLFWwindow*) {
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_CLOSED, nullptr);
}

void TMessageManager::WindowRefreshCallback(GLFWwindow*) {
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_REFRESHED, nullptr);
}

void TMessageManager::WindowFocusCallback(GLFWwindow*, int focused) {
    if(focused) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_RECEIVED_FOCUS, nullptr);
    } else {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_LOST_FOCUS, nullptr);
    }
}

void TMessageManager::WindowIconifyCallback(GLFWwindow*, int iconified) {
    struct Data{
        int iconified;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->iconified = iconified;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_ICONIFY_CHANGED, data);
}

void TMessageManager::FramebufferSizeCallback(GLFWwindow*, int width, int height) {
    struct Data{
        int width, height;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->width = width;
    data->height = height;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::WINDOW_FRAMEBUFFER_SIZE_CHANGED, data);
}

void TMessageManager::MouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
    struct Data{
        int button, action, mods;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->button = button;
    data->action = action;
    data->mods = mods;
    if(action == GLFW_PRESS) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MOUSE_BUTTON_PRESSED, data);
    } else if(action == GLFW_RELEASE) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MOUSE_BUTTON_RELEASED, data);
    }
}

void TMessageManager::CursorPosCallback(GLFWwindow*, double x, double y) {
    struct Data{
        float x, y;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->x = (float)x;
    data->y = Application::sThisApplication->GetAppInfo()->windowHeight - (float)y;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MOUSE_POSITION_CHANGED, data);
}

void TMessageManager::CursorEnterCallback(GLFWwindow*, int entered) {
    struct Data{
        int entered;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->entered = entered;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MOUSE_ENTERED_WINDOW, data);
}

void TMessageManager::ScrollCallback(GLFWwindow*, double x, double y) {
    struct Data{
        float x, y;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->x = (float)x;
    data->y = (float)y;
    Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::SCROLL_CHANGED, data);
}

void TMessageManager::KeyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
    struct Data{
        int key, scancode, action, mods;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->key = key;
    data->scancode = scancode;
    data->action = action;
    data->mods = mods;
    if(action == GLFW_PRESS) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::KEY_PRESSED, data);
    } else if(action == GLFW_RELEASE) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::KEY_RELEASED, data);
    }
}

void TMessageManager::MonitorCallback(GLFWmonitor *monitor, int action) {
    struct Data{
        GLFWmonitor *monitor;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->monitor = monitor;
    if(action == GLFW_CONNECTED) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MONITOR_CONNECTED, data);
    } else if(action == GLFW_DISCONNECTED) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::MONITOR_DISCONNECTED, data);
    }
}

void TMessageManager::JoystickCallback(int jid, int action) {
    struct Data {
        int jid;
    };
    auto data = (Data*)malloc(sizeof(Data));
    data->jid = jid;
    if(action == GLFW_CONNECTED) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::JOYSTICK_CONNECTED, data);
    } else if(action == GLFW_DISCONNECTED) {
        Singleton<TMessageManager>::GetInstance()->SendMessageEvent(Events::JOYSTICK_DISCONNECTED, data);
    }
}
