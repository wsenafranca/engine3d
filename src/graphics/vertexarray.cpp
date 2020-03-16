//
// Created by Washington on 22/02/2020.
//

#include "vertexarray.hpp"
#include "opengl.hpp"

VertexArray::VertexArray() : glArrayId(0) {
    glGenVertexArrays(1, &glArrayId);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &glArrayId);
}

void VertexArray::EnableAttrib(uint32_t index) {
    glEnableVertexAttribArray(index);
}

void VertexArray::DisableAttrib(uint32_t index) {
    glDisableVertexAttribArray(index);
}

void VertexArray::SetAttribPointer(uint32_t index, int32_t size, uint32_t type, bool normalized, uint32_t stride,
                                   const void *pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VertexArray::SetAttribIPointer(uint32_t index, int32_t size, uint32_t type, uint32_t stride, const void *pointer) {
    glVertexAttribIPointer(index, size, type, stride, pointer);
}

void VertexArray::SetAttribLPointer(uint32_t index, int32_t size, uint32_t type, uint32_t stride, const void *pointer) {
    glVertexAttribLPointer(index, size, type, stride, pointer);
}

void VertexArray::Bind() {
    glBindVertexArray(glArrayId);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}
