//
// Created by Washington on 07/03/2020.
//

#include "luamanager.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <controllers/animcontroller.hpp>

void TLuaManager::Init() {
    mLuaState = std::make_unique<sol::state>();
    mLuaState->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::io);

    mLuaState->new_usertype<glm::vec3>("Vec3",
            sol::constructors<glm::vec3(), glm::vec3(float, float, float), glm::vec3(const glm::vec3&)>(),
            sol::meta_function::index, [](glm::vec3& v, sol::stack_object key, sol::this_state L) {
                auto maybe_string_key = key.as<sol::optional<std::string>>();
                if (maybe_string_key) {
                    const std::string& k = *maybe_string_key;
                    if (k == "x") {
                        return sol::object(L, sol::in_place, v.x);
                    }
                    else if (k == "y") {
                        return sol::object(L, sol::in_place, v.y);
                    }
                    else if (k == "z") {
                        return sol::object(L, sol::in_place, v.z);
                    }
                }

                auto maybe_numeric_key = key.as<sol::optional<int>>();
                if (maybe_numeric_key) {
                    int n = *maybe_numeric_key;
                    switch (n) {
                        case 0:
                            return sol::object(L, sol::in_place, v.x);
                        case 1:
                            return sol::object(L, sol::in_place, v.y);
                        case 2:
                            return sol::object(L, sol::in_place, v.z);
                        default:
                            break;
                    }
                }

                return sol::object(L, sol::in_place, sol::lua_nil);
            }
    );

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

    mLuaState->new_usertype<AnimController>("AnimController",
            sol::meta_function::index, [](AnimController& controller, sol::stack_object key, sol::this_state L) {
                auto maybe_string_key = key.as<sol::optional<std::string>>();
                if (maybe_string_key) {
                    const std::string& k = *maybe_string_key;
                    if(k == "name") {
                        return sol::object(L, sol::in_place, controller.GetName());
                    } else if(controller.HasProperty(k)) {
                        const auto& value = controller.GetProperties().at(k);
                        if(value.type() == typeid(int)) {
                            return sol::object(L, sol::in_place, std::any_cast<int>(value));
                        } else if(value.type() == typeid(bool)) {
                            return sol::object(L, sol::in_place, std::any_cast<bool>(value));
                        } else if(value.type() == typeid(float)) {
                            return sol::object(L, sol::in_place, std::any_cast<float>(value));
                        } else if(value.type() == typeid(std::string)) {
                            return sol::object(L, sol::in_place, std::any_cast<std::string>(value));
                        }
                    }
                }
                return sol::object(L, sol::in_place, sol::lua_nil);
            });

    LoadScript("data/lua/animcontrollers.lua");
}

void TLuaManager::LoadScript(const filesystem::path &file) {
    if(!file.exists()) {
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str());
    }

    mLuaState->script_file(file.str());
}

sol::state *TLuaManager::GetLuaState() {
    return mLuaState.get();
}
