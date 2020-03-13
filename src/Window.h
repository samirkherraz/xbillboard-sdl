#ifndef Window_H
#define Window_H

#include <vector>
#include "Screen.h"
#include "ScreenThread.h"
#include "Config.h"
#include <mutex>
#include <condition_variable>
class Window
{
public:
    Window(int, int);
    ~Window();
    Screen *getScreen(unsigned int);
    bool instance(int screen, ScreenDefinition *);
    void start();
    static void askDraw();
    static void lock();
    static void unlock();

private:
    void clear();
    void wait();
    void notify();
    SDL_Renderer *renderer;
    SDL_Window *window;
    static std::mutex mtx;
    static std::mutex gmtx;
    static std::condition_variable cond;
    static bool job_availiable;
    static bool draw_finished;
    vector<Screen> screens;
    vector<SDL_Thread *> player_threads;
    vector<SDL_Thread *> downloader_threads;
};
#endif
