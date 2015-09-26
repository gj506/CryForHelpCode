//Granville Jones
//Intro to Game Programming

#include "ClassDemoApp.h"

int main(int argc, char *argv[])
{
	ClassDemoApp app;
	app.Setup();
	while (!app.UpdateAndRender()){}
	return 0;
}