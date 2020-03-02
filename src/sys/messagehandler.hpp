//
// Created by Washington on 23/02/2020.
//

#ifndef ENGINE3D_SRC_MESSAGEHANDLER_HPP
#define ENGINE3D_SRC_MESSAGEHANDLER_HPP

#include <future>
#include <functional>
#include <unordered_map>
#include "messagemanager.hpp"
#include "events.hpp"

#define BIND_EVENT_CALLBACK(event, obj, method)  do {\
    mFilter[event] = [obj](int ev, void* data) {obj->method(ev, data);}; \
    MessageManager::AddHandler(event, obj); \
    } while(0)

#define UNBIND_CALLBACK(obj) MessageManager::RemoveHandler(obj)

class MessageHandler {
public:
    virtual ~MessageHandler();
    void OnMessageReceived(int event, void* data);

protected:
    std::unordered_map<int, std::function<void(int, void*)> > mFilter;
};


#endif //ENGINE3D_SRC_MESSAGEHANDLER_HPP
