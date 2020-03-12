//
// Created by Washington on 07/03/2020.
//

#include "luamanager.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <components/animator.hpp>
#include <sys/input.hpp>

void TLuaManager::Init() {
    mLuaState = std::make_unique<sol::state>();
    mLuaState->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::io);

    mLuaState->new_usertype<AnimState>("AnimState",
            sol::meta_function::index, [](AnimState& animState, sol::stack_object key, sol::this_state L) {
                auto maybe_string_key = key.as<sol::optional<std::string>>();
                if (maybe_string_key) {
                    const std::string& k = *maybe_string_key;
                    if (k == "isFinished") {
                        return sol::object(L, sol::in_place, animState.isFinished);
                    }
                }
                return sol::object(L, sol::in_place, sol::lua_nil);
            });

    mLuaState->new_usertype<Animator>("Animator",
        "SetPropertyBool", [](Animator& controller, const std::string &key, const bool &value) {
            controller.SetProperty<bool>(key, value);
        },
        "SetPropertyFloat", [](Animator& controller, const std::string &key, const float &value) {
            controller.SetProperty<float>(key, value);
        },
        "SetPropertyInt", [](Animator& controller, const std::string &key, const int &value) {
            controller.SetProperty<int>(key, value);
        },
        "SetPropertyString", [](Animator& controller, const std::string &key, const std::string &value) {
            controller.SetProperty<std::string>(key, value);
        },
        "GetPropertyBool", [](Animator& controller, const std::string &key) {
            return controller.GetProperty<bool>(key).value_or(false);
        },
        "GetPropertyFloat", [](Animator& controller, const std::string &key) {
            return controller.GetProperty<float>(key).value_or(0.0f);
        },
        "GetPropertyInt", [](Animator& controller, const std::string &key) {
            return controller.GetProperty<int>(key).value_or(0);
        },
        "GetPropertyString", [](Animator& controller, const std::string &key) {
            return controller.GetProperty<std::string>(key).value_or("");
        });

    LoadFile("data/lua/animators/init.lua");
}

sol::unsafe_function_result TLuaManager::LoadFile(const filesystem::path &file) {
    if(!file.exists()) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
    }

    return mLuaState->script_file(file.str());
}

std::string TLuaManager::GetName(const filesystem::path &file) {
    return file.filename().substr(0, file.filename().rfind('.'));
}

sol::state *TLuaManager::GetLuaState() {
    return mLuaState.get();
}
