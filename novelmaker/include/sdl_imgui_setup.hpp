#ifndef SDL_IMGUI_RENDER_
#define SDL_IMGUI_RENDER_

#include "../3rdparty/imgui/imgui.h"
#include "../3rdparty/imgui/imgui_impl_sdl2.h"

namespace NM
{
    SDL_Window* createSDL_Window(float main_scale);
    SDL_Renderer* createSDL_Renderer(SDL_Window*);
    
    ImGuiIO& setupImGuiContext();
    void setupImGuiRendererBackend(SDL_Window* window, SDL_Renderer* renderer);
    
    // Style editing
    void setupFonts(ImGuiIO& io);
    void setupStyle(float main_scale);
}

#endif // SDL_IMGUI_SETUP_
