#include "Window.h"

std::condition_variable Window::cond;
std::mutex Window::mtx;
std::mutex Window::gmtx;

bool Window::job_availiable = false;
bool Window::draw_finished = false;

Screen *Window::getScreen(unsigned int i)
{
    if (i < screens.size())
    {
        return &screens[i];
    }
    return NULL;
}

bool Window::instance(int screen, ScreenDefinition *sd)
{
    ScreenThread *th = new ScreenThread(getScreen(screen), sd);
    std::string plname = "PL_" + sd->getName();
    SDL_Thread *player = SDL_CreateThread(ScreenThread::play, plname.c_str(), th);
    if (NULL == player)
    {
        return false;
    }
    else
    {
        player_threads.push_back(player);
    }
    std::string dlname = "DL_" + sd->getName();
    SDL_Thread *downloader = SDL_CreateThread(ScreenThread::download, dlname.c_str(), th);
    if (NULL == downloader)
    {
        return false;
    }
    else
    {
        downloader_threads.push_back(downloader);
    }
    return true;
}

void Window::start()
{
    while (true)
    {
        wait();
        for (Screen screen : screens)
        {
            screen.getQueue()->lock();
        }
        Window::lock();
        SDL_RenderClear(renderer);
        for (Screen screen : screens)
        {
            if (screen.getQueue()->getTexture() != NULL)
                SDL_RenderCopy(renderer, screen.getQueue()->getTexture(), NULL, screen.getQueue()->getRect());
        }
        SDL_RenderPresent(renderer);
        Window::unlock();
        for (Screen screen : screens)
        {
            screen.getQueue()->unlock();
        }
        notify();
    }
}

Window::Window(int nbx, int nby)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        exit(-1);
    }
    else
    {
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);
        av_register_all();
        window = SDL_CreateWindow(
            "XBillBoard", 0, 0, current.w, current.h, SDL_WINDOW_OPENGL);

        if (window)
        {
            SDL_GL_SwapWindow(window);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int screen_w = current.w / nbx;
            int screen_h = current.h / nby;

            for (int i = 0; i < nby; ++i)
            {
                for (int j = 0; j < nbx; ++j)
                {
                    SDL_Rect rect;
                    rect.x = j * screen_w;
                    rect.y = i * screen_h;
                    rect.w = screen_w;
                    rect.h = screen_h;
                    screens.push_back(Screen(rect, renderer));
                }
            }
        }
    }
}

Window::~Window()
{
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::askDraw()
{
    std::unique_lock<std::mutex> lk(Window::mtx);
    Window::job_availiable = true;
    Window::draw_finished = false;
    Window::cond.notify_all();
    Window::cond.wait(lk, [] { return Window::draw_finished; });
}

void Window::notify()
{
    std::unique_lock<std::mutex> lk(Window::mtx);

    Window::draw_finished = true;
    Window::job_availiable = false;
    Window::cond.notify_all();
}

void Window::wait()
{
    std::unique_lock<std::mutex> lk(Window::mtx);
    Window::cond.wait(lk, [] { return Window::job_availiable; });
}

void Window::lock()
{
    Window::gmtx.lock();
}

void Window::unlock()
{
    Window::gmtx.unlock();
}
