#include "Screen.h"

Screen::Screen(SDL_Rect _rect, SDL_Renderer *_renderer)
{
    rect = _rect;
    renderer = _renderer;
    queue = new Drawable();
}

void Screen::drawImage(std::string path, int delay)
{
    Image *img = new Image(path, rect, renderer, queue);
    if (img->init())
    {
        img->start(delay);
    }
    delete img;
}
void Screen::drawPdf(std::string path, int delay)
{
    Pdf *pdf = new Pdf(path, rect, renderer, queue);
    if (pdf->init())
    {
        pdf->start(delay);
    }
    delete pdf;
}

void Screen::drawVideo(std::string path)
{
    Video *vid = new Video(path, rect, renderer, queue);
    if (vid->init())
    {
        vid->start();
    }
    delete vid;
}

void Screen::drawLogo(unsigned int delay)
{
    Image *img = new Image("./xbillboard.svg", rect, renderer, queue);
    if (img->init())
    {
        img->start(delay);
    }
    delete img;
}
SDL_Rect &Screen::getRect()
{
    return rect;
}

Drawable *Screen::getQueue()
{
    return queue;
}