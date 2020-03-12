#include "Pdf.h"
#include "Window.h"
Pdf::~Pdf()
{
    if (document)
    {
        delete document;
    }
}

void Pdf::do_nothing(const std::string &, void *)
{
}

bool Pdf::init()
{
    poppler::set_debug_error_function(this->do_nothing, this);

    document = poppler::document::load_from_file(path);
    page_renderer.set_render_hint(page_renderer.text_antialiasing);
    if (!document)
    {
        return false;
    }
    nbPages = document->pages();
    return true;
}

void Pdf::start(int delay)
{
    for (int i = 0; i < nbPages; ++i)
    {
        poppler::page *page = document->create_page(i);
        poppler::image page_image = page_renderer.render_page(page, 96.0, 96.0);
        SDL_Surface *surface = SDL_CreateRGBSurface(0, page_image.width(), page_image.height(), 32, 0, 0, 0, 0);
        const char *pixels = page_image.const_data();
        int imagesize = page_image.bytes_per_row() * page_image.height();
        for (int pindex = 0; pindex < imagesize; pindex++)
        {
            *((char *)(surface->pixels) + pindex) = pixels[pindex];
        }
        SDL_Surface *imageSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(surface);

        queue->lock();
        queue->setTarget(calculateRenderRect(page_image.width(), page_image.height()));
        queue->setTexture(SDL_CreateTextureFromSurface(renderer, imageSurface));
        queue->unlock();

        fadeIn();
        SDL_Delay(delay);

        SDL_FreeSurface(imageSurface);
    }
}
