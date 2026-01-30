#ifndef IMAGE_HANDLER_
#define IMAGE_HANDLER_

#include <cstdint>

#include <SDL2/SDL.h>

struct Image
{
    const char* file_name;

    uint8_t* data;
    size_t data_size;

    SDL_Texture* texture;
    int width;
    int height;
    int channels;
};

Image createImage(const char* file_name, SDL_Renderer* renderer);

#endif // IMAGE_HANDLER_
