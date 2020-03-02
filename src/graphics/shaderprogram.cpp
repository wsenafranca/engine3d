//
// Created by Washington on 21/02/2020.
//

#include "shaderprogram.hpp"

#include <regex>
#include <fstream>
#include "opengl.hpp"
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(glProgram);
}

void ShaderProgram::AddSource(uint32_t type, const std::string &source) {
    auto parsed = ParseSource(source);

    char *buffer = (char*) malloc(parsed.length()+1);
    strcpy(buffer, parsed.c_str());
    buffer[parsed.length()] = 0;

    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &buffer, nullptr);
    glCompileShader(s);
    std::free(buffer);
    int success;
    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if(!success) {
        char log[1024];
        glGetShaderInfoLog(s, 1024, nullptr, log);
        throw std::runtime_error(log);
    }
    mShaders.push_back(s);
}

void ShaderProgram::AddFile(uint32_t type, const filesystem::path &file) {
    auto source = LoadSourceFile(file);
    try {
        AddSource(type, source);
    }
    catch(std::exception& e) {
        throw std::runtime_error(file.str() + ": " + e.what());
    }
}

void ShaderProgram::Compile() {
    glProgram = glCreateProgram();
    for(auto& s : mShaders) {
        glAttachShader(glProgram, s);
    }
    glLinkProgram(glProgram);
    for(auto& s : mShaders) {
        glDeleteShader(s);
    }
    mShaders.clear();
}

void ShaderProgram::Use() {
    glUseProgram(glProgram);
}

std::string ShaderProgram::ParseSource(const std::string &source) {
    const std::regex includePattern("#include\\s*<(.*)>");
    const std::regex newLinePattern("\n");
    std::string parsed_source;

    std::sregex_token_iterator it(source.begin(), source.end(), newLinePattern, -1);
    std::sregex_token_iterator end;
    std::list< std::string> lines;
    for(; it != end; ++it) {
        lines.push_back(it->str());
    }

    for(const auto& line : lines) {
        std::smatch m;
        if(std::regex_match(line, m, includePattern)) {
            std::string filename = m[1];
            filesystem::path file = "shaders/" + filename;
            parsed_source += ParseSource(LoadSourceFile(file)) + "\n";
        } else {
            parsed_source += line + "\n";
        }
    }

    return parsed_source;
}

std::string ShaderProgram::LoadSourceFile(const filesystem::path &file) {
    if(!file.exists()) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
    }
    std::ifstream stream(file.str());
    std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return source;
}

GLint ShaderProgram::GetUniformLocation(const std::string &name) const {
    auto it = mUniformCache.find(name);
    if(it == mUniformCache.end()) {
        GLint loc = glGetUniformLocation(glProgram, name.c_str());
        if(loc < 0) {
            //throw std::runtime_error("uniform " + name + " not found.");
            return loc;
        }
        mUniformCache[name] = loc;
        return loc;
    }
    return it->second;
}

GLint ShaderProgram::GetAttribLocation(const std::string &name) const {
    auto it = mAttribCache.find(name);
    if(it == mAttribCache.end()) {
        GLint loc = glGetAttribLocation(glProgram, name.c_str());
        if(loc < 0) {
            return loc;
        }
        mAttribCache[name] = loc;
        return loc;
    }
    return it->second;
}

void ShaderProgram::SetUniform(const std::string &name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string &name, uint32_t value) {
    glUniform1ui(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string &name, int32_t value) {
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string &name, const glm::vec3 &v) {
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(v));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::vec4 &v) {
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(v));
}

void ShaderProgram::SetUniform(const std::string &name, const glm::mat4 &m) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::SetUniform(const std::string &name, const Material &material) {
    glUniform1i(GetUniformLocation(name + ".diffuseTexture"), MATERIAL_TEXTURE_DIFFUSE_SLOT);
    glUniform1i(GetUniformLocation(name + ".specularTexture"), MATERIAL_TEXTURE_SPECULAR_SLOT);
    glUniform1i(GetUniformLocation(name + ".occlusionTexture"), MATERIAL_TEXTURE_OCCLUSION_SLOT);
    glUniform1i(GetUniformLocation(name + ".normalTexture"), MATERIAL_TEXTURE_NORMAL_SLOT);
    glUniform1i(GetUniformLocation(name + ".heightTexture"), MATERIAL_TEXTURE_NORMAL_SLOT);
    if(material.diffuseTexture) {
        material.diffuseTexture->BindActive(MATERIAL_TEXTURE_DIFFUSE_SLOT);
    }
    if(material.specularTexture) {
        material.specularTexture->BindActive(MATERIAL_TEXTURE_SPECULAR_SLOT);
    }
    if(material.occlusionTexture) {
        material.occlusionTexture->BindActive(MATERIAL_TEXTURE_OCCLUSION_SLOT);
    }
    if(material.normalTexture) {
        material.normalTexture->BindActive(MATERIAL_TEXTURE_NORMAL_SLOT);
    }
    if(material.heightTexture) {
        material.heightTexture->BindActive(MATERIAL_TEXTURE_HEIGHT_SLOT);
    }
    glUniform3fv(GetUniformLocation(name + ".diffuseColor"), 1, glm::value_ptr(material.diffuseColor));
    glUniform3fv(GetUniformLocation(name + ".specularColor"), 1, glm::value_ptr(material.specularColor));
    glUniform3fv(GetUniformLocation(name + ".ambientColor"), 1, glm::value_ptr(material.ambientColor));
    glUniform1f(GetUniformLocation(name + ".shininess"), material.shininess);
    glUniform1f(GetUniformLocation(name + ".opacity"), material.opacity);
    glUniform1ui(GetUniformLocation(name + ".textureBits"), material.textureBits);
}

void ShaderProgram::SetUniform(const std::string &name, const Light &light) {
    glUniform1i(GetUniformLocation(name + ".type"), light.type);
    glUniform3fv(GetUniformLocation(name + ".position"), 1, glm::value_ptr(light.position));
    glUniform3fv(GetUniformLocation(name + ".direction"), 1, glm::value_ptr(light.direction));
    glUniform3fv(GetUniformLocation(name + ".color"), 1, glm::value_ptr(light.color));
    glUniform1f(GetUniformLocation(name + ".constantAttenuation"), light.constantAttenuation);
    glUniform1f(GetUniformLocation(name + ".linearAttenuation"), light.linearAttenuation);
    glUniform1f(GetUniformLocation(name + ".quadraticAttenuation"), light.quadraticAttenuation);
    glUniform1f(GetUniformLocation(name + ".spotInnerCutoff"), std::cos(light.spotInnerAngle));
    glUniform1f(GetUniformLocation(name + ".spotOuterCutoff"), std::cos(light.spotOuterAngle));
}
