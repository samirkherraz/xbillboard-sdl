#ifndef Screen_H
#define Screen_H
#include <iostream>
#include <SDL2/SDL.h>
#include "Media.h"
#include "Video.h"
#include "Pdf.h"
#include "Image.h"
using namespace std;

class Screen
{
public:
    Screen(SDL_Rect, SDL_Renderer *);
    void drawImage(std::string path, int delay);
    void drawPdf(std::string path, int delay);
    void drawVideo(std::string path);
    void drawLogo(unsigned int delay);
    SDL_Rect& getRect();
    Drawable* getQueue();
    void clear();
  
private:
    SDL_Rect rect;
    SDL_Renderer *renderer;
    Drawable* queue;
};
#endif
