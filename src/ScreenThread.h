#ifndef ScreenThread_H
#define ScreenThread_H
#include "Screen.h"
#include "Types.h"

class ScreenThread
{
public:
    ScreenThread(Screen *, ScreenDefinition *);
    int static play(void *);
    int static download(void *);

private:
    void playFiles();
    void downloadFiles();
    ScreenDefinition *sd;
    Screen *screen;
    FILE_TYPE types;
    const bool CONVERT = true;
};
#endif
