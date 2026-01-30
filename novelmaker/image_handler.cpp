#include "include/image_handler.hpp"

#include "include/load_texture_impl.hpp"
#include <SDL2/SDL_render.h>


Image createImage(const char* file_name, SDL_Renderer* renderer)
{
     Image image{ .file_name = file_name };

     loadTextureFromFile(file_name, &image.data, renderer, &image.texture
                       , &image.width, &image.height, &image.channels);

     return image;
}
