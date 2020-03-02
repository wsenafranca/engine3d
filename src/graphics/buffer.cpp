//
// Created by Washington on 21/02/2020.
//

#include "buffer.hpp"
#include "opengl.hpp"

Buffer::Buffer(uint32_t target) : glBufferId(0), target(target) {
    glGenBuffers(1, &glBufferId);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &glBufferId);
}

void Buffer::SetData(size_t size, const void *data, uint32_t usage) {
    glBufferData(target, size, data, usage);
}

void Buffer::SetSubData(size_t offset, size_t size, const void *data) {
    glBufferSubData(target, offset, size, data);
}

void Buffer::Bind() {
    glBindBuffer(target, glBufferId);
}

void Buffer::Unbind() {
    glBindBuffer(target, 0);
}
