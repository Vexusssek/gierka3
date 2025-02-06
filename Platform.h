#pragma once
#include <string>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    inline int kbhitWrapper() 
    {
        return _kbhit();
    }
#else
    #include <termios.h>
    #include <unistd.h>
    int kbhitWrapper();
#endif

void clearScreen();
int getchWrapper();
void setupLocale();
