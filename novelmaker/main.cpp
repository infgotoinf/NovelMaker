#define DEVELOPER_OPTIONS // Disable this for a release

//#include <string>

#include <SDL.h>
#include <SDL_syswm.h>
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_impl_sdl2.h"
#include "3rdparty/imgui/imgui_impl_sdlrenderer2.h"

#include "include/sdl_imgui_setup.hpp"

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

//-------------------------------------------------------------------------------------------------
//      STATE
//-------------------------------------------------------------------------------------------------

    // Making app to take entire screen
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int display_width = dm.w;
    int display_height = dm.h; 
    SDL_SetWindowSize(window, display_width/2, display_height/2);
    SDL_SetWindowPosition(window, display_width/2, display_height/2);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool show_demo_window = true;
    int counter = 0;
    float f = 0;

//=================================================================================================
//      START OF THE MAIN LOOP
//=================================================================================================

    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui
        // wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main
        //   application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main
        //   application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your
        // application based on those two flags.
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

//-------------------------------------------------------------------------------------------------
//          MAIN WINDOW
//-------------------------------------------------------------------------------------------------

        ImGuiWindowFlags main_window_flags = 0;
        main_window_flags = ImGuiWindowFlags_NoScrollbar
                          | ImGuiWindowFlags_NoDecoration
                          | ImGuiWindowFlags_NoMove
                          | ImGuiWindowFlags_NoSavedSettings
                          | ImGuiWindowFlags_NoBringToFrontOnFocus;


        static bool use_work_area = true;

        // We demonstrate using the full viewport area or the work area
        // (without menu-bars, task-bars etc.)
        // Based on your use case you may want one or the other.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//          MAIN WINDOW CONTENT
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    ImGui::Begin("Novel Maker", nullptr, main_window_flags);
    {
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        
        ImGui::Text("Text");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
    ImGui::End();

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//          CHILD WINDOWS
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        ImGui::SetNextWindowSize(ImVec2(500, 400));

    #ifdef DEVELOPER_OPTIONS
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
    #endif

//-------------------------------------------------------------------------------------------------
//          RENDERING
//-------------------------------------------------------------------------------------------------
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
