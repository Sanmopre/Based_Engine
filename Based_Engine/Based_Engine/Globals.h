#ifndef _GLOBALS_
#define _GLOBALS_

#include <windows.h>
#include <stdio.h>

#include <vector>
#include <string>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);
std::vector<std::string> GetLogs();
void EraseGetLogs();

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 810
#define SCREEN_SIZE 1
#define HISTOGRAM_SIZE 60
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
//#define FPS 60
#define TITLE "BASED Engine"

#endif