#pragma once
#include "Globals.h"

std::vector<std::string> logs;

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	logs.push_back(tmp_string);

	int a = 0;
}

std::vector<std::string> GetLogs()
{
	return logs;
}

void EraseGetLogs()
{
	logs.erase(logs.begin(), logs.end());
}