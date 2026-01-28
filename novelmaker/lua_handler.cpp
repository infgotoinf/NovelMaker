#include "include/lua_handler.hpp"

#include <string>
#include <vector>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

DialogueLine::DialogueLine(const sol::table& luaLine)
{
    if (luaLine["characters"].valid())
    {
        sol::table charsTable = luaLine["characters"];
        for (size_t i = 1; i <= charsTable.size(); ++i)
        {
            characters.push_back(charsTable[i]);
        }
    }
    
    if (luaLine["text"].valid())
    {
        text = luaLine["text"];
    }
}

Scene::Scene(const sol::table& luaScene)
{
    background = luaScene["background"];
    
    if (luaScene.size() > 0)
    {
        sol::table dialoguesTable = luaScene[1];
        for (size_t i = 1; i <= dialoguesTable.size(); ++i)
        {
            sol::table dialogueTable = dialoguesTable[i];
            dialogues.emplace_back(dialogueTable);
        }
    }
}

Novel::Novel(const sol::table& luaNovel)
{
    for (const auto& pair : luaNovel) {
        if (pair.second.is<sol::table>()) {
            scenes.emplace_back(pair.second.as<sol::table>());
        }
    }
}


namespace NM
{
    sol::state createLuaState()
    {
        sol::state lua;
        lua.open_libraries();
    
        return lua;
    }

    Novel loadNovelFromLuaFile(sol::state lua, const char* path_to_file)
    {
        sol::protected_function_result result = lua.script_file(path_to_file);
        if (!result.valid())
        {
            sol::error err = result;
            std::cerr << "Lua error: " << err.what() << std::endl;
            std::exit(1);
        }
    
        sol::function getNovel = result;
        sol::table LuaNovel = getNovel();
        
        Novel novel(LuaNovel);

        return novel;
    }
}
