#include "Media.h"
#include "Window.h"
Media::Media(std::string _path, SDL_Rect _rect, SDL_Renderer *_renderer, Drawable *_queue)
{
    queue = _queue;
    path = _path;
    renderer = _renderer;
    renderer_rect = _rect;
}

SDL_Rect Media::calculateRenderRect(double w, double h)
{
    SDL_Rect targetRect;
    double ratio;
    if (w >= h)
    {
        ratio = double(renderer_rect.w) / double(w);
    }
    else
    {
        ratio = double(renderer_rect.h) / double(h);
    }
    targetRect.w = w * ratio;
    targetRect.h = h * ratio;
    targetRect.x = renderer_rect.x + (double(double(renderer_rect.w) - double(targetRect.w)) / 2);
    targetRect.y = renderer_rect.y + (double(double(renderer_rect.h) - double(targetRect.h)) / 2);

    return targetRect;
}

void Media::show()
{
    Window::askDraw();
}
void Media::fadeIn()
{
    queue->lock();
    SDL_SetTextureBlendMode(queue->getTexture(), SDL_BLENDMODE_BLEND);
    queue->unlock();
    for (int i = 0; i <= 255; i += 15)
    {
        queue->lock();
        SDL_SetTextureAlphaMod(queue->getTexture(), i);
        queue->unlock();
        show();
        SDL_Delay(20);
    }
}

void Media::fadeOut()
{
    queue->lock();
    SDL_SetTextureBlendMode(queue->getTexture(), SDL_BLENDMODE_BLEND);
    queue->unlock();
    for (int i = 255; i >= 0; i -= 15)
    {

        queue->lock();
        SDL_SetTextureAlphaMod(queue->getTexture(), i);
        queue->unlock();
        show();
        SDL_Delay(20);
    }
}
