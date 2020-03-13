#ifndef Media_H
#define Media_H

#include "Types.h"

class Media
{
public:
    Media(std::string _path, SDL_Rect _rect, SDL_Renderer *_renderer, Drawable *_queue);
    bool init();
    void start(int delay);
    void fadeIn();
    void fadeOut();
    void show();

protected:
    SDL_Rect calculateRenderRect(double w, double h);
    string path;
    SDL_Rect renderer_rect;
    SDL_Renderer *renderer;
    Drawable *queue;
};

#endif
