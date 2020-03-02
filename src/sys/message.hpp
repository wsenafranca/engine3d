//
// Created by Washington on 23/02/2020.
//

#ifndef ENGINE3D_SRC_MESSAGE_HPP
#define ENGINE3D_SRC_MESSAGE_HPP

#include <cstring>
#include <memory>

struct Message {
    int event;
    void* data;
};

#endif //ENGINE3D_SRC_MESSAGE_HPP
