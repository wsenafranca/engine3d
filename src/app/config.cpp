//
// Created by Washington on 29/02/2020.
//

#include "config.hpp"
#include "../../lib/inih/INIReader.h"
#include <fstream>

namespace internal {
    class INIReader_ : public INIReader {
    public:
        explicit INIReader_(const std::string& filename) : INIReader(filename) {}
        [[nodiscard]] const std::map<std::string, std::string>& GetProperties() const {
            return _values;
        }
    };
}

void Config::LoadFile(const filesystem::path &file) {
    if(!file.exists()) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
    }
    auto reader = internal::INIReader_(file.str());
    auto ret = reader.ParseError();
    if(ret != 0) {
        throw std::runtime_error("Error parsing INI file " + file.str());
    }

    for(const auto& section : reader.Sections()) {
        for(const auto& p : reader.GetProperties()) {
            mData[section.empty() ? p.first : section+"."+p.first] = p.second;
        }
    }
}

void Config::SaveFile(const filesystem::path &file) {
    std::ofstream stream(file.str());
    stream.exceptions ( std::ofstream::badbit );
    try {
        for(const auto& it : mData) {
            stream << it.first << " = " << it.second << std::endl;
        }
    } catch (const std::ofstream::failure& e) {
        throw std::ofstream::failure("Error saving file " + file.str() + " " + e.what());
    }
}

void Config::Put(const std::string &property, int value) {
    mData[property] = std::to_string(value);
}

void Config::Put(const std::string &property, float value) {
    mData[property] = std::to_string(value);
}

void Config::Put(const std::string &property, bool value) {
    mData[property] = std::to_string(value);
}

void Config::Put(const std::string &property, const std::string &value) {
    mData[property] = value;
}

std::optional<int> Config::GetInt(const std::string &property) const {
    try {
        return std::stol(mData.at(property));
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<float > Config::GetFloat(const std::string &property) const {
    try {
        return std::stol(mData.at(property));
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool > Config::GetBool(const std::string &property) const {
    try {
        auto value = mData.at(property);
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == "true" || value == "yes" || value == "on" || value == "1")
            return true;
        else if (value == "false" || value == "no" || value == "off" || value == "0")
            return false;
        else
            throw std::exception();
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<std::string> Config::GetString(const std::string &property) const {
    try {
        return mData.at(property);
    } catch (...) {
        return std::nullopt;
    }
}

bool Config::HasProperty(const std::string &property) const {
    return mData.find(property) != mData.end();
}

std::list<std::string> Config::GetProperties() const {
    std::list<std::string> list;
    for(const auto& it : mData) {
        list.push_back(it.first);
    }
    return list;
}
