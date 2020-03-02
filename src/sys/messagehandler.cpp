//
// Created by Washington on 23/02/2020.
//

#include "messagehandler.hpp"

MessageHandler::~MessageHandler() {
    MessageManager::RemoveHandler(this);
}

void MessageHandler::OnMessageReceived(int event, void *data) {
    auto filter = mFilter[event];
    if(filter) filter(event, data);
}
