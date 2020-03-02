//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_BUFFER_HPP
#define ENGINE3D_SRC_BUFFER_HPP

#include <cstdint>

class Buffer {
public:
    explicit Buffer(uint32_t target);
    ~Buffer();

    void SetData(size_t size, const void* data, uint32_t usage);
    void SetSubData(size_t offset, size_t size, const void* data);

    void Bind();
    void Unbind();

    uint32_t glBufferId;
    uint32_t target;
};

#endif //ENGINE3D_SRC_BUFFER_HPP
