//
// Created by Washington on 29/02/2020.
//

#ifndef ENGINE3D_SRC_APPINFO_HPP
#define ENGINE3D_SRC_APPINFO_HPP

#include <cstdint>
#include <string>
#include <vector>

struct AppInfo {
    std::string appName;
    int windowPositionX;
    int windowPositionY;
    uint32_t windowWidth;
    uint32_t windowHeight;
    std::vector<std::string> args;
};

#endif //ENGINE3D_SRC_APPINFO_HPP
