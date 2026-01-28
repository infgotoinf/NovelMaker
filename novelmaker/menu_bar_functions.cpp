#include "include/menu_bar_functions.hpp"

#include <string>
#include <filesystem>
#include <fstream>

#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_internal.h"
#include "3rdparty/imgui/imgui_stdlib.h"
#include "3rdparty/imgui_fd/ImGuiFileDialog.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "3rdparty/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdparty/stb/stb_image_write.h"

#include "../assets/text_images/default.h"
#include "../assets/novel_lua.h"

namespace fs = std::filesystem;


void openProject(std::string novel_lua_path, std::string* novel_lua_text_data)
{
    std::ifstream novel_lua_file(novel_lua_path);
    *novel_lua_text_data = "";
    std::string text;
    while (getline(novel_lua_file, text))
    {
        *novel_lua_text_data += text + '\n';
    }
    novel_lua_file.close();
}

bool showProjectOpeningWindow(bool* p_open, std::string* novel_lua_text_data)
{
    std::string project_location = ".";
    IGFD::FileDialogConfig config;config.path = project_location;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose project location directory"
                                          , nullptr, config);

    bool ret_code = true;
    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            project_location = ImGuiFileDialog::Instance()->GetCurrentPath();

            std::string novel_lua_path = project_location + "\\novel.lua";

            if (fs::is_regular_file(novel_lua_path))
            {
                openProject(novel_lua_path, novel_lua_text_data);
            }
            else {
                ret_code = false;
            }
        }

        ImGuiFileDialog::Instance()->Close();
        *p_open = false;

    }
    return ret_code;
}


void browseLocation(bool* p_open, std::string* project_location)
{
    IGFD::FileDialogConfig config;config.path = *project_location;
    ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose project location directory"
                                          , nullptr, config);

    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            *project_location = ImGuiFileDialog::Instance()->GetCurrentPath();
        }

        ImGuiFileDialog::Instance()->Close();
        *p_open = false;
    }
}

bool createProject(std::string project_location, std::string* novel_lua_path)
{
    if (!fs::create_directory(project_location))
    {
        return false;
    }
    std::string images_dir_location = project_location + "\\images";
    fs::create_directory(images_dir_location);

    *novel_lua_path = project_location + "\\novel.lua";
    std::ofstream novel_lua_file(*novel_lua_path);
    novel_lua_file << novel_lua_text_data << std::endl;
    novel_lua_file.close();
    

    std::string images_backgrounds_dir_location = images_dir_location + "\\backgrounds";
    fs::create_directory(images_backgrounds_dir_location);

    int b_x = 960;
    int b_y = 540;
    std::string cur_image_location;
    std::string cur_dir;
    cur_image_location = images_backgrounds_dir_location + "\\forest.jpg";
    stbi_write_jpg(cur_image_location.c_str(), b_x, b_y, 3, backgrounds_forest, 90);
    cur_image_location = images_backgrounds_dir_location + "\\winter_mountain.jpg";
    stbi_write_jpg(cur_image_location.c_str(), b_x, b_y, 3, backgrounds_winter_mountain, 90);
    cur_image_location = images_backgrounds_dir_location + "\\danils_home1.png";
    stbi_write_png(cur_image_location.c_str(), b_x, b_y, 3, backgrounds_danils_home1, b_x * 3);
    cur_image_location = images_backgrounds_dir_location + "\\danils_home2.png";
    stbi_write_png(cur_image_location.c_str(), b_x, b_y, 3, backgrounds_danils_home2, b_x * 3);
    cur_image_location = images_backgrounds_dir_location + "\\black_screen.png";
    stbi_write_png(cur_image_location.c_str(), b_x, b_y, 3, backgrounds_black_screen, b_x * 3);


    std::string images_characters_dir_location = images_dir_location + "\\characters";
    fs::create_directory(images_characters_dir_location);
    
    int c_xy = 540;
    std::string danil_dir_location = images_characters_dir_location + "\\danil";
    fs::create_directory(danil_dir_location);
    cur_image_location = danil_dir_location + "\\agree.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_agree, c_xy * 4);
    cur_image_location = danil_dir_location + "\\disagree.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_disagree, c_xy * 4);
    cur_image_location = danil_dir_location + "\\exited.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_exited, c_xy * 4);
    cur_image_location = danil_dir_location + "\\looking_up.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_looking_up, c_xy * 4);
    cur_image_location = danil_dir_location + "\\sleeping.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_sleeping, c_xy * 4);
    cur_image_location = danil_dir_location + "\\waking_up.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, danil_waking_up, c_xy * 4);

    std::string shark_guy_dir_location = images_characters_dir_location + "\\shark_guy";
    fs::create_directory(shark_guy_dir_location);
    cur_image_location = shark_guy_dir_location + "\\TwT.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, shark_guy_TwT, c_xy * 4);
    cur_image_location = shark_guy_dir_location + "\\angry.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, shark_guy_angry, c_xy * 4);
    cur_image_location = shark_guy_dir_location + "\\grinning.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, shark_guy_grinning, c_xy * 4);
    cur_image_location = shark_guy_dir_location + "\\scarry.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, shark_guy_scarry, c_xy * 4);

    std::string linus_torvalds_dir_location = images_characters_dir_location + "\\linus_torvalds";
    fs::create_directory(linus_torvalds_dir_location);
    cur_image_location = linus_torvalds_dir_location + "\\based.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, linus_torvalds_based, c_xy * 4);
    cur_image_location = linus_torvalds_dir_location + "\\scarry.png";
    stbi_write_png(cur_image_location.c_str(), c_xy, c_xy, 4, linus_torvalds_scarry, c_xy * 4);

    return true;
}


void showNewProjectCreationWindow(bool* p_open, std::string* novel_lua_text_data)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings
                                  | ImGuiWindowFlags_AlwaysAutoResize
                                  | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Create new project", p_open, window_flags);
    {
        static std::string project_name = "NewProject";
        ImGui::InputText("Project name", &project_name, ImGuiInputTextFlags_CharsNoBlank);

        
        static std::string project_location = fs::current_path().string();
        ImGui::InputText("Project location", &project_location);
        ImGui::SameLine();
        
        static bool show_browse_location_window = false;
        static std::string path_to_project_dir;

        if (ImGui::Button("..."))
        {
            show_browse_location_window = true;
        }

        if (show_browse_location_window)
            browseLocation(&show_browse_location_window, &project_location);

        ImGui::Spacing();

        bool project_name_entered = project_name.size() != 0;
        bool location_is_valid = fs::is_directory(project_location);
        bool data_correct = location_is_valid && project_name_entered;
        if (data_correct)
        {
            path_to_project_dir = project_location + "\\" + project_name;
            ImGui::TextWrapped("Project will be created at: %s", path_to_project_dir.c_str());
        }
        ImGui::Separator();

        
        if (ImGui::Button("Cancel"))
        {
            *p_open = false;
        }
        ImGui::SameLine();
       
        // Setting button to the right
        static float confirm_button_width = ImGui::CalcTextSize("Confirm").x
                                          + ImGui::GetStyle().FramePadding.x * 2.f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x
                           - confirm_button_width);

        // Making button disabled
        if (data_correct == false)
            ImGui::BeginDisabled();

        static bool project_created;
        if (ImGui::Button("Confirm"))
        {
            std::string novel_lua_path = "";
            project_created = createProject(path_to_project_dir, &novel_lua_path);
            if (project_created)
            {
                *p_open = false;
                openProject(novel_lua_path, novel_lua_text_data);
            }
        }

        if (data_correct == false)
            ImGui::EndDisabled();

        ImGui::End();
    }
}

