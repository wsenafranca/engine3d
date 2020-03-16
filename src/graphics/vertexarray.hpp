//
// Created by Washington on 22/02/2020.
//

#ifndef ENGINE3D_SRC_VERTEXARRAY_HPP
#define ENGINE3D_SRC_VERTEXARRAY_HPP

#include <cstdint>

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void EnableAttrib(uint32_t index);
    void DisableAttrib(uint32_t index);
    void SetAttribPointer(uint32_t index, int32_t size, uint32_t type, bool normalized, uint32_t stride = 0, const void *pointer = nullptr);
    void SetAttribIPointer(uint32_t index, int32_t size, uint32_t type, uint32_t stride = 0, const void *pointer = nullptr);
    void SetAttribLPointer(uint32_t index, int32_t size, uint32_t type, uint32_t stride = 0, const void *pointer = nullptr);

    void Bind();
    void Unbind();

    uint32_t glArrayId;
};


#endif //ENGINE3D_SRC_VERTEXARRAY_HPP
