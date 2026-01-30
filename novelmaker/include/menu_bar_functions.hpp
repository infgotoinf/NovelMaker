#ifndef MENU_BAR_FUNCTIONS_
#define MENU_BAR_FUNCTIONS_

#include <string>

bool showProjectOpeningWindow(bool* p_open, std::string* novel_lua_text_data
                            , std::string* path_to_project_dir);

void showNewProjectCreationWindow(bool* p_open, std::string* novel_lua_text_data
                                   , std::string* path_to_project_dir);

std::string openProject();

#endif // MENU_BAR_FUNCTIONS_
