#include "Config.h"

using namespace std;

Config::Config(string configFile)
{
    screens = new vector<ScreenDefinition *>();
    bool global = false;
    int screen_id = -1;
    int file_id = 0;
    ifstream infile(configFile);
    string line;

    while (getline(infile, line))
    {
        if (line.length() > 0 && line[0] != '#')
        {
            string key, value;
            int pos = line.find_first_of("=");
            if (pos > 0)
            {
                key = line.substr(0, pos);
                value = line.substr(pos + 1);
                if (global)
                {
                    if (key == "x")
                    {
                        x = atoi(value.c_str());
                    }
                    else if (key == "y")
                    {
                        y = atoi(value.c_str());
                    }
                }
                else
                {

                    if (key == "file")
                    {
                        file_id++;
                        ScreenFileDefinition *f = new ScreenFileDefinition();
                        f->setUrl(value);
                        int e = f->getUrl().find_last_of(".");
                        string ext = f->getUrl().substr(e + 1);
                        f->setPath(screens->at(screen_id)->getPath() + "/file_" + to_string(file_id) + "." + ext);
                        f->setType(getFileType(f->getPath()));
                        screens->at(screen_id)->getFiles()->push_back(f);
                    }
                    else if (key == "delay")
                    {
                        screens->at(screen_id)->setDelay(atoi(value.c_str()) * 1000);
                    }
                    else if (key == "loop")
                    {
                        screens->at(screen_id)->setLoop(atoi(value.c_str()));
                    }
                }
            }
            else
            {
                if (line == "[global]")
                {
                    global = true;
                }
                else if (line == "[screen]")
                {
                    global = false;
                    screen_id++;
                    screens->push_back(new ScreenDefinition());
                    screens->at(screen_id)->setName("screen_" + to_string(screen_id));
                    screens->at(screen_id)->setPath(string(string("/tmp/") + screens->at(screen_id)->getName()));
                    file_id = 0;
                }
            }
        }
    }
}

Config::~Config()
{
    for (ScreenDefinition *s : *screens)
    {
        for (ScreenFileDefinition *f : *s->getFiles())
        {
            delete f;
        }
        delete s;
    }
    delete screens;
}

FILE_TYPE Config::getFileType(string filepath)
{
    int position = string(filepath).find_last_of(".");
    string ext = filepath.substr(position + 1);
    transform(ext.begin(), ext.end(), ext.begin(), ::toupper);
    if (ext == "PDF")
    {
        return PDF;
    }
    if (ext == "JPG")
    {
        return IMAGE;
    }
    if (ext == "JPEG")
    {
        return IMAGE;
    }
    if (ext == "PNG")
    {
        return IMAGE;
    }
    if (ext == "SVG")
    {
        return IMAGE;
    }
    if (ext == "MP4")
    {
        return VIDEO;
    }
    if (ext == "AVI")
    {
        return VIDEO;
    }
    if (ext == "3GP")
    {
        return VIDEO;
    }
    return NONE;
}

unsigned int Config::getX()
{
    return x;
}
unsigned int Config::getY()
{
    return y;
}

unsigned int Config::getCount()
{
    return x * y;
}
ScreenDefinition *Config::getScreen(unsigned int i)
{
    return screens->at(i);
}

void Config::dump()
{
    cout << "X               : " << getX() << endl;
    cout << "Y               : " << getY() << endl;
    for (ScreenDefinition *s : *screens)
    {

        cout << "Screen          : " << s->getName() << endl;
        cout << "   Path         : " << s->getPath() << endl;
        cout << "   Delay        : " << s->getDelay() << endl;
        cout << "   Loop         : " << s->getLoop() << endl;
        cout << "   Files        : " << endl;
        for (ScreenFileDefinition *f : *s->getFiles())
        {
            cout << "       - " << f->getUrl() << endl;
            cout << "           Path : " << f->getPath() << endl;
            cout << "           Type : " << f->getType() << endl;
            cout << "           Ready: " << f->isReady() << endl;
        }
    }
}