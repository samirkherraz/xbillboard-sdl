#ifndef Config_H
#define Config_H
#include "Types.h"
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

class Config
{
public:
    unsigned int getX();
    unsigned int getY();
    unsigned int getCount();
    void dump();
    FILE_TYPE getFileType(string filepath);
    ScreenDefinition *getScreen(unsigned int i);
    Config(string);
    ~Config();

private:
    unsigned int x = 1;
    unsigned int y = 1;
    vector<ScreenDefinition *> *screens;
};
#endif
