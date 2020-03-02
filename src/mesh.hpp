//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_MESH_HPP
#define ENGINE3D_SRC_MESH_HPP

#include "object.hpp"
#include <glm/glm.hpp>
#include "buffer.hpp"
#include "vertexarray.hpp"
#include "material.hpp"
#include <vector>
#include <memory>

class MeshBuilder;

enum MeshAttributeIndex {
    ATTRIBUTE_POSITION_INDEX = 0,
    ATTRIBUTE_NORMAL_INDEX = 1,
    ATTRIBUTE_TEXCOORD_INDEX = 2,
    ATTRIBUTE_JOINTS_INDEX = 3,
    ATTRIBUTE_WEIGHTS_INDEX = 4,
    ATTRIBUTE_TANGENT_INDEX = 5,
};

enum MeshAttributeBits {
    ATTRIBUTE_POSITION_BITS = 1u << 0u,
    ATTRIBUTE_NORMAL_BITS = 1u << 1u,
    ATTRIBUTE_TEXCOORD_BITS = 1u <<  2u,
    ATTRIBUTE_JOINTS_BITS = 1u << 3u,
    ATTRIBUTE_WEIGHTS_BITS = 1u << 4u,
    ATTRIBUTE_TANGENT_BITS = 1u << 5u
};

struct Mesh {
    explicit Mesh(MeshBuilder* builder);

    std::string name;
    std::vector< std::unique_ptr<Buffer> > buffers;
    Material material;
    uint32_t renderMode{4};
    uint32_t indexType{5125};
    uint32_t indexCount{0};
    std::unique_ptr<VertexArray> vertexArray{};
    uint32_t attributeBits{0};
};



#endif //ENGINE3D_SRC_MESH_HPP
