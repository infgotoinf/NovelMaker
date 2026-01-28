#define DEVELOPER_OPTIONS // Disable this for a release

#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_syswm.h>
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

    bool show_demo_window = true;
    bool show_new_project_window = false;
    bool show_open_project_window = false;
    std::string novel_lua_text_data = "";

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
        #endif
            ImGui::InputTextMultiline("##novel_lua", &novel_lua_text_data
                                    , ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16)
                                    , input_flags);
        }
        ImGui::End();

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//          CHILD WINDOWS
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        //ImGui::SetNextWindowSize(ImVec2(500, 400));

        //ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x/2, viewport->WorkPos.y/2));

        static bool opening_project_exists = true;

        if (show_open_project_window)
            opening_project_exists =
                    showProjectOpeningWindow(&show_open_project_window, &novel_lua_text_data);

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
            showNewProjectCreationWindow(&show_new_project_window, &novel_lua_text_data);


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
