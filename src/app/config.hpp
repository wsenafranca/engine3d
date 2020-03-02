//
// Created by Washington on 29/02/2020.
//

#ifndef ENGINE3D_SRC_CONFIG_HPP
#define ENGINE3D_SRC_CONFIG_HPP

#include <string>
#include <list>
#include <filesystem/path.h>
#include <unordered_map>
#include <optional>

class Config {
public:
    void LoadFile(const filesystem::path& file);
    void SaveFile(const filesystem::path& file);

    void Put(const std::string& property, int value);
    void Put(const std::string& property, float value);
    void Put(const std::string& property, bool value);
    void Put(const std::string& property, const std::string& value);

    [[nodiscard]] std::optional<int> GetInt(const std::string& property) const;
    [[nodiscard]] std::optional<float> GetFloat(const std::string& property) const;
    [[nodiscard]] std::optional<bool> GetBool(const std::string& property) const;
    [[nodiscard]] std::optional< std::string> GetString(const std::string& property) const;

    [[nodiscard]] bool HasProperty(const std::string& property) const;
    [[nodiscard]] std::list<std::string> GetProperties() const;

private:
    std::unordered_map<std::string, std::string> mData;
};


#endif //ENGINE3D_SRC_CONFIG_HPP
