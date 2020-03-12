#include "ScreenThread.h"
#include "Window.h"

using namespace std;

void ScreenThread::playFiles()
{
    bool found = false;
    for (unsigned int i = 0; i < sd->getFiles()->size(); ++i)
    {
        if (sd->getFiles()->at(i)->isReady())
        {
            sd->getFiles()->at(i)->lock();
            switch (sd->getFiles()->at(i)->getType())
            {
            case IMAGE:
                found = true;
                screen->drawImage(sd->getFiles()->at(i)->getPath(), sd->getDelay());
                break;
            case VIDEO:
                found = true;
                screen->drawVideo(sd->getFiles()->at(i)->getPath());
                break;
            case PDF:
                found = true;
                screen->drawPdf(sd->getFiles()->at(i)->getPath(), sd->getDelay());
                break;

            default:
                screen->drawLogo(sd->getDelay());
            }
            sd->getFiles()->at(i)->unlock();
        }
    }
    if (!found)
    {
        screen->drawLogo(sd->getDelay());
    }
}
void ScreenThread::downloadFiles()
{
    string cmd = "mkdir -p " + sd->getPath();
    system(cmd.c_str());
    for (unsigned int i = 0; i < sd->getFiles()->size(); ++i)
    {
        cmd = "wget -q '";
        cmd += sd->getFiles()->at(i)->getUrl();
        cmd += "'";
        cmd += " -O '";
        cmd += sd->getFiles()->at(i)->getPath();
        if ((CONVERT) && (sd->getFiles()->at(i)->getType() == VIDEO))
        {
            cmd += ".old";
        }
        else
        {
            sd->getFiles()->at(i)->setReady(false);
        }
        cmd += "'";
        if (system(cmd.c_str()) == 0)
        {
            if ((CONVERT) && (sd->getFiles()->at(i)->getType() == VIDEO))
            {
                string ffmpeg = "ffmpeg -i '";
                ffmpeg += sd->getFiles()->at(i)->getPath();
                ffmpeg += ".old' -an -y -vcodec h264 -r 30 -filter:v scale=";
                ffmpeg += to_string(screen->getRect().w / 2);
                ffmpeg += ":-1 '";
                ffmpeg += sd->getFiles()->at(i)->getPath();
                ffmpeg += "' > /dev/null 2>&1";
                sd->getFiles()->at(i)->setReady(false);
                if (system(ffmpeg.c_str()) == 0)
                {
                    sd->getFiles()->at(i)->setReady(true);
                }
            }
            else
            {
                sd->getFiles()->at(i)->setReady(true);
            }
        }
    }
}

int ScreenThread::play(void *ptr)
{
    SDL_Delay(3000);
    while (true)
    {
        static_cast<ScreenThread *>(ptr)->playFiles();
    }
    return 0;
}

int ScreenThread::download(void *ptr)
{
    while (true)
    {
        static_cast<ScreenThread *>(ptr)->downloadFiles();
        SDL_Delay(300000);
    }
    return 0;
}

ScreenThread::ScreenThread(Screen *sc, ScreenDefinition *p)
{
    screen = sc;
    sd = p;
}