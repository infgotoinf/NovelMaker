#include "include/sdl_imgui_setup.hpp"

#include <SDL.h>
#include <SDL_syswm.h>
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_impl_sdl2.h"
#include "3rdparty/imgui/imgui_impl_sdlrenderer2.h"

#include "../assets/fonts/ProggyVector.h"

namespace NM
{
//-------------------------------------------------------------------------------------------------
//          SETUP
//-------------------------------------------------------------------------------------------------
    SDL_Window* createSDL_Window(float main_scale)
    {
    #ifdef _WIN32
        ::SetProcessDPIAware();
    #endif
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            std::exit(1);
        }

        // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

        SDL_WindowFlags window_flags = (SDL_WindowFlags)
                (SDL_WINDOW_RESIZABLE
               | SDL_WINDOW_ALLOW_HIGHDPI);

        SDL_Window* window = SDL_CreateWindow("Novel Maker"
                                            , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
                                            , (int)(1280 * main_scale), (int)(800 * main_scale)
                                            , window_flags);
        if (window == nullptr)
        {
            printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
            std::exit(1);
        }
        return window;
    }

    SDL_Renderer* createSDL_Renderer(SDL_Window* window)
    {
        SDL_RendererFlags renderer_flags = (SDL_RendererFlags)
                (SDL_RENDERER_PRESENTVSYNC
               | SDL_RENDERER_ACCELERATED);

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, renderer_flags);
        if (renderer == nullptr)
        {
            SDL_Log("Error creating SDL_Renderer!");
            std::exit(1);
        }
        return renderer;
    }

    void setupImGuiRendererBackend(SDL_Window* window, SDL_Renderer* renderer)
    {
        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
    ImGuiIO& setupImGuiContext()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard      // Enable Keyboard Controls
                        | ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        return io;
    }

//-------------------------------------------------------------------------------------------------
//          STYLE EDITING
//-------------------------------------------------------------------------------------------------
    void setupFonts(ImGuiIO& io)
    {
        // Load Fonts
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        builder.BuildRanges(&ranges);
        io.Fonts->AddFontFromMemoryCompressedTTF(ProggyVector_compressed_data
                                               , ProggyVector_compressed_size, 16.0f
                                               , nullptr, ranges.Data);
    }

    void setupStyle(float main_scale)
    {
        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
    
        // Bake a fixed style scale.
        // (until we have a solution for dynamic style scaling, changing this requires resetting
        // Style + calling this again)
        style.ScaleAllSizes(main_scale);
    
        // Set initial font scale.
        // (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for
        // documentation purpose)
        style.FontScaleDpi = main_scale;
    
        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_TextDisabled]           = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.04f, 0.04f, 0.04f, 0.30f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.50f, 0.50f, 0.50f, 0.30f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.64f, 0.64f, 0.64f, 0.50f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.78f, 0.78f, 0.78f, 0.70f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.52f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 0.61f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 0.71f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 0.81f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.78f, 0.78f, 0.78f, 0.70f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.68f, 0.68f, 0.68f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.82f, 0.82f, 0.82f, 0.50f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.96f, 0.96f, 0.96f, 0.60f);
        colors[ImGuiCol_Header]                 = ImVec4(0.50f, 0.50f, 0.50f, 0.30f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.64f, 0.64f, 0.64f, 0.50f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.78f, 0.78f, 0.78f, 0.70f);
        colors[ImGuiCol_Separator]              = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.78f, 0.78f, 0.78f, 0.80f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.50f, 0.50f, 0.50f, 0.40f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
        colors[ImGuiCol_TabSelected]            = ImVec4(0.78f, 0.78f, 0.78f, 0.80f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    
        style.GrabRounding   = 3.0f;
        style.FrameRounding  = 3.0f;
    }
}
