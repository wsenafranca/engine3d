//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_SHADERPROGRAM_HPP
#define ENGINE3D_SRC_SHADERPROGRAM_HPP

#include <filesystem/path.h>
#include <list>
#include <unordered_map>
#include <glm/glm.hpp>
#include "material.hpp"
#include "light.hpp"

class ShaderProgram {
public:
    template<class... Args>
    explicit ShaderProgram(Args&& ...args) : glProgram(0) {
        std::list<std::string> shaders{args...};
        for(auto &filename : shaders) {
            auto file = filesystem::path("data/shaders/" + filename);
            auto ext = file.extension();
            uint32_t type;
            if(ext == "comp") {
                type = 0x91B9;
            } else if(ext == "vert") {
                type = 35633;
            } else if(ext == "tesc") {
                type = 36488;
            } else if(ext == "tese") {
                type = 36487;
            } else if(ext == "geom") {
                type = 36313;
            } else if(ext == "frag") {
                type = 35632;
            } else {
                throw std::runtime_error("The shader file " + file.str() + " must have a valid extension");
            }
            AddFile(type, file);
        }
    }
    ~ShaderProgram();

    void AddSource(uint32_t type, const std::string& source);
    void AddFile(uint32_t type, const filesystem::path& file);

    void Compile();
    void Use();

    int32_t GetUniformLocation(const std::string& name) const;
    int32_t GetAttribLocation(const std::string& name) const;

    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, uint32_t value);
    void SetUniform(const std::string& name, int32_t value);
    void SetUniform(const std::string& name, const glm::vec3& v);
    void SetUniform(const std::string& name, const glm::vec4& v);
    void SetUniform(const std::string& name, const glm::mat4& m);
    void SetUniform(const std::string& name, const Material& material);
    void SetUniform(const std::string& name, const Light& light);

    uint32_t glProgram;

protected:
    static std::string ParseSource(const std::string& source);
    static std::string LoadSourceFile(const filesystem::path& file);

private:
    std::list<uint32_t> mShaders;
    mutable std::unordered_map<std::string, int32_t> mUniformCache;
    mutable std::unordered_map<std::string, int32_t> mAttribCache;
};

#endif //ENGINE3D_SRC_SHADERPROGRAM_HPP
