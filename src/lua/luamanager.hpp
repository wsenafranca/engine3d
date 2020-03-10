//
// Created by Washington on 07/03/2020.
//

#ifndef ENGINE3D_SRC_LUA_LUAMANAGER_HPP
#define ENGINE3D_SRC_LUA_LUAMANAGER_HPP

#include "lua.hpp"
#include <sol/sol.hpp>
#include <memory>
#include <filesystem/path.h>
#include <base/singleton.hpp>

class TLuaManager {
public:
    void Init();
    void LoadScript(const filesystem::path& file);

    sol::state *GetLuaState();
private:
    std::unique_ptr<sol::state> mLuaState;
};

namespace LuaManager {
    inline void Init() {Singleton<TLuaManager>::GetInstance()->Init();}
    inline void LoadScript(const filesystem::path& file) {Singleton<TLuaManager>::GetInstance()->LoadScript(file);}
    inline sol::state *GetLuaState() {return Singleton<TLuaManager>::GetInstance()->GetLuaState();}
}

#endif //ENGINE3D_SRC_LUA_LUAMANAGER_HPP
