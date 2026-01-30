//#define DEVELOPER_OPTIONS // Disable this for a release

#include <cstddef>
#include <exception>
#include <vector>
#include <string>
#include <fstream>

#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_impl_sdl2.h"
#include "3rdparty/imgui/imgui_impl_sdlrenderer2.h"
#include "3rdparty/imgui/imgui_stdlib.h"

#include "include/sdl_imgui_setup.hpp"
#include "include/lua_handler.hpp"
#include "include/menu_bar_functions.hpp"
#include "include/image_handler.hpp"


struct Step
{
    std::vector<Image> ch_images;
    Image bg_image;

    std::string text;  
};

std::vector<Step> novelToSteps(Novel novel, std::string path_to_project, SDL_Renderer* renderer)
{
    std::vector<Step> steps;
    for (Scene scene : novel.scenes)
    {
        std::string path_to_bg = path_to_project + scene.background;
        for (DialogueLine dialogue : scene.dialogues)
        {
            std::vector<Image> ch_images;
            for (std::string ch : dialogue.characters)
            {
                std::string path_to_ch = path_to_project + ch;
                ch_images.push_back(createImage(path_to_ch.c_str(), renderer));
            }
            Step step = {.ch_images = ch_images
                       , .bg_image = createImage(path_to_bg.c_str(), renderer)
                       , .text = dialogue.text};

            steps.push_back(step);
        }
    }
    return steps;
}

//=================================================================================================
//      START OF THE MAIN CODE
//=================================================================================================

int main(int, char**)
{
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

    SDL_Window* window     = NM::createSDL_Window(main_scale);
    SDL_Renderer* renderer = NM::createSDL_Renderer(window);

    ImGuiIO& io = NM::setupImGuiContext();

    NM::setupImGuiRendererBackend(window, renderer);

    NM::setupFonts(io);
    NM::setupStyle(main_scale);

    sol::state lua = NM::createLuaState();

//-------------------------------------------------------------------------------------------------
//      STATE
//-------------------------------------------------------------------------------------------------

#ifdef DEVELOPER_OPTIONS
    bool show_demo_window = true;
#endif
    bool show_new_project_window = false;
    bool show_open_project_window = false;
    std::string novel_lua_text_data = "";
    std::string path_to_project = "";
    Novel novel{};
    
    std::vector<Step> steps;

    size_t step_id = 0;
    size_t counter = 1;

//=================================================================================================
//      START OF THE MAIN LOOP
//=================================================================================================

    bool done = false;
    while (!done)
    {
        // You basically don't want to touch this part
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

//=================================================================================================
//          APPLICATION INTERFACE
//=================================================================================================
//          WINDOW MENU BAR
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem("New"))
                {
                    show_new_project_window = true;
                }
                if (ImGui::MenuItem("Open"))
                {
                    show_open_project_window = true;
                }
                if (ImGui::MenuItem("Recent"))
                {

                }
                if (ImGui::MenuItem("Save..."))
                {
                    
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//          MAIN WINDOW
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        static ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoDecoration
                                                  | ImGuiWindowFlags_NoMove
                                                  | ImGuiWindowFlags_NoSavedSettings
                                                  | ImGuiWindowFlags_NoBringToFrontOnFocus;

        static ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_AllowTabInput
                                               | ImGuiInputTextFlags_WordWrap
                                               | ImGuiInputTextFlags_CtrlEnterForNewLine;

        static bool use_work_area = true;

        static const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        ImGui::Begin("Novel Maker", nullptr, main_window_flags);
        {
        #ifdef DEVELOPER_OPTIONS
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::SameLine();
        #endif
            ImGui::BeginDisabled(path_to_project.size() == 0);
            if (ImGui::Button("Save"))
            {
                std::string novel_lua_path = path_to_project + "\\novel.lua";
                std::ofstream novel_lua_file(novel_lua_path);
                novel_lua_file << novel_lua_text_data << std::endl;
                novel_lua_file.close();
                novel = NM::loadNovelFromLuaCode(lua, novel_lua_text_data);
                steps = novelToSteps(novel, path_to_project, renderer);
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            static bool show_config = true;
            if (ImGui::Button("Hide config"))
            {
                show_config = !show_config;
            }

            if (show_config)
            {
                ImGui::InputTextMultiline("##novel_lua", &novel_lua_text_data
                                        , ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16)
                                        , input_flags);
                ImGui::Spacing();
                ImGui::Text("Path to project: %s", path_to_project.c_str());
            }
            ImGui::Separator();
            float preview_begin = ImGui::GetCursorPosY();

            if (!steps.empty())
            {
                Step cur_step = steps[step_id];
                Image cur_bg = cur_step.bg_image;
                float bg_x = ImGui::GetContentRegionAvail().x;
                float resize_coef = bg_x / cur_bg.width;
                float bg_y = resize_coef * cur_bg.height;
                ImGui::Image(cur_bg.texture, ImVec2 {bg_x, bg_y});

                int number_of_ch_images = cur_step.ch_images.size();
                for (int i = 0 ; i < number_of_ch_images; ++i)
                {
                    Image cur_ch_image = cur_step.ch_images[i];
                    float ch_x = resize_coef * cur_ch_image.width;
                    float ch_y = resize_coef * cur_ch_image.height;
                    ImGui::SetCursorPosY(preview_begin);
                    ImGui::SetCursorPosX(((bg_x - number_of_ch_images * ch_x)
                                       / (number_of_ch_images + 1)) * (i + 1) + i * ch_x);
                    ImGui::Image(cur_ch_image.texture, ImVec2 {ch_x, ch_y});
                }
                float old_size = ImGui::GetFont()->Scale;
                ImGui::GetFont()->Scale *= 3 * resize_coef;
                ImGui::PushFont(ImGui::GetFont());
                ImGui::TextWrapped(cur_step.text.substr(0, counter).c_str());
                if (counter != cur_step.text.size())
                    ++counter;
                ImGui::GetFont()->Scale = old_size;
                ImGui::PopFont();
            }           
        }
        ImGui::End();

        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)
          | ImGui::IsKeyPressed(ImGuiKey_D)
          | ImGui::IsKeyPressed(ImGuiKey_L))
        {
            if(step_id < (steps.size() - 1))
            {
                ++step_id;
                counter = 1;
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)
          | ImGui::IsKeyPressed(ImGuiKey_A)
          | ImGui::IsKeyPressed(ImGuiKey_H))
        {
            if(step_id > 0)
            {
                --step_id;
                counter = 1;
            }
        }

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//          CHILD WINDOWS
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        //ImGui::SetNextWindowSize(ImVec2(500, 400));

        //ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x/2, viewport->WorkPos.y/2));

        static bool opening_project_exists = true;

        if (show_open_project_window)
        {
            std::string old_novel_lua_text_data = novel_lua_text_data;
            opening_project_exists =
                    showProjectOpeningWindow(&show_open_project_window, &novel_lua_text_data
                                           , &path_to_project);
            if (old_novel_lua_text_data != novel_lua_text_data)
            {
                novel = NM::loadNovelFromLuaCode(lua, novel_lua_text_data);
                steps = novelToSteps(novel, path_to_project, renderer);
            }
        }

        if (opening_project_exists == false)
        {    
            static ImGuiWindowFlags error_window_flags = ImGuiWindowFlags_NoSavedSettings
                                                       | ImGuiWindowFlags_AlwaysAutoResize
                                                       | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("Error", nullptr, error_window_flags);
            ImGui::Text("Coudn't find 'novel.lua' in the project's root folder!");
            if (ImGui::Button("Ok"))
            {
                opening_project_exists = true;
            }
            ImGui::End();
        }

        if (show_new_project_window)
        {
            std::string old_novel_lua_text_data = novel_lua_text_data;
            showNewProjectCreationWindow(&show_new_project_window, &novel_lua_text_data
                                       , &path_to_project);
            if (old_novel_lua_text_data != novel_lua_text_data)
            {
                novel = NM::loadNovelFromLuaCode(lua, novel_lua_text_data);
                steps = novelToSteps(novel, path_to_project, renderer);
            }
        }


    #ifdef DEVELOPER_OPTIONS
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    #endif

//=================================================================================================
//          RENDERING
//=================================================================================================
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        SDL_SetRenderDrawColor(renderer
                             , (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255)
                             , (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
