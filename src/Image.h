#ifndef Image_H
#define Image_H

#include "Media.h"
#include <SDL2/SDL_image.h>
class Image : protected Media
{
public:
    Image(std::string _path, SDL_Rect _rect, SDL_Renderer *_renderer, Drawable *_queue) : Media(_path, _rect, _renderer, _queue){};
    ~Image();
    bool init();
    void start(int delay);

private:
    SDL_Surface *imageSurface;
};
#endif
