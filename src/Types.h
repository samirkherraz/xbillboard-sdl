#ifndef Types_H
#define Types_H

#include <vector>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

enum FILE_TYPE
{
    PDF,
    VIDEO,
    IMAGE,
    NONE
};

class ScreenFileDefinition
{
public:
    ScreenFileDefinition(){

    };
    ~ScreenFileDefinition(){

    };
    string getUrl()
    {
        return url;
    }
    string getPath()
    {
        return path;
    }
    FILE_TYPE getType()
    {
        return type;
    }
    void setUrl(string u)
    {
        url = u;
    }
    void setPath(string p)
    {
        path = p;
    }
    void setType(FILE_TYPE t)
    {
        type = t;
    }
    bool isReady()
    {
        lock();
        bool r = ready;
        unlock();
        return r;
    }
    void lock()
    {
        mtx.lock();
    }
    void unlock()
    {
        mtx.unlock();
    }
    void setReady(bool r)
    {
        lock();
        ready = r;

        unlock();
    }

private:
    string url = "";
    string path = "";
    FILE_TYPE type;
    std::mutex mtx;
    bool ready;
};

class ScreenDefinition
{
public:
    ScreenDefinition()
    {
        files = new vector<ScreenFileDefinition *>();
    }
    string getName()
    {
        return name;
    }
    unsigned int getLoop()
    {
        return loop;
    }
    unsigned int getDelay()
    {
        return delay;
    }
    string getPath()
    {
        return path;
    }
    vector<ScreenFileDefinition *> *getFiles()
    {
        return files;
    }
    void setName(string n)
    {
        name = n;
    }
    void setLoop(unsigned int l)
    {
        loop = l;
    }
    void setDelay(unsigned int d)
    {
        delay = d;
    }
    void setPath(string p)
    {
        path = p;
    }

private:
    string name = "screen_#";
    unsigned int loop = 1;
    unsigned int delay = 1;
    string path;
    vector<ScreenFileDefinition *> *files;
};

class Drawable
{

public:
    Drawable()
    {
        texture = NULL;
    }
    ~Drawable()
    {
        if (texture != NULL)
        {
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
    }

    void setTexture(SDL_Texture *t)
    {

        if (texture != NULL && t != NULL)
        {
            SDL_DestroyTexture(texture);
            texture = t;
        }
        else if (t != NULL)
        {
            texture = t;
        }
    }

    void setTarget(SDL_Rect t)
    {
        target = t;
    }

    SDL_Texture *getTexture()
    {
        return texture;
    }

    SDL_Rect *getRect()
    {
        return &target;
    }

    void lock()
    {
        mtx.lock();
    }
    void unlock()
    {
        mtx.unlock();
    }

private:
    SDL_Texture *texture = NULL;
    SDL_Texture *tmp = NULL;
    SDL_Rect target;
    std::mutex mtx;
};

#endif
