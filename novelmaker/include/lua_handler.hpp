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
};

namespace NM
{
    sol::state createLuaState();
    Novel openLuaFile(sol::state& lua);
}

#endif // LUA_HANDLER_
