#include <iostream>
#include "Window.h"
#include "Config.h"
using namespace std;

int main(int argc, char **argv)
{

	Config cfg("./exemple.conf");
	cfg.dump();

	Window main(cfg.getX(), cfg.getY());

	for (unsigned int i = 0; i < cfg.getCount(); ++i)
	{
		if (!main.instance(i, cfg.getScreen(i)))
		{
			exit(100);
		}
	}
	main.start();
}
