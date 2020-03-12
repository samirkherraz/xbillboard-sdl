#include "Image.h"
#include "Window.h"
bool Image::init()
{

    SDL_Surface *surface = IMG_Load(path.c_str());
    imageSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);
    if (!imageSurface)
    {
        return false;
    }
    return true;
}

void Image::start(int delay)
{
    queue->lock();
    queue->setTarget(calculateRenderRect(imageSurface->w, imageSurface->h));
    queue->setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface));
    queue->unlock();

    fadeIn();
    SDL_Delay(delay);
}

Image::~Image()
{
    if (imageSurface)
    {
        SDL_FreeSurface(imageSurface);
    }
}