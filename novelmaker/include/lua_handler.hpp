#ifndef LUA_HANDLER_
#define LUA_HANDLER_

#include <string>
#include <vector>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

struct DialogueLine
{
    std::vector<std::string> characters;
    std::string text;

    DialogueLine(const sol::table& luaLine);
};

struct Scene
{
    std::string background;
    std::vector<DialogueLine> dialogues;

    Scene(const sol::table& luaScene); 
};

struct Novel
{
    std::vector<Scene> scenes;

    Novel(const sol::table& luaNovel);
    Novel() = default; 
    Novel& operator=(const Novel& n) = default;
};

namespace NM
{
    sol::state createLuaState();
    Novel loadNovelFromLuaCode(sol::state& lua, const std::string& lua_code);
}

#endif // LUA_HANDLER_
