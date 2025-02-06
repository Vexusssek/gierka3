#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#endif

class ConsoleBuffer 
{
private:
    int width, height;
#ifdef _WIN32
    HANDLE hConsole;
    COORD bufferSize;
    CHAR_INFO* screenBuffer;
    SMALL_RECT writeRegion;
#else
    std::vector<std::string> screenBuffer;
#endif

public:
    ConsoleBuffer(int w, int h);
    ~ConsoleBuffer();
    void clear();
    void write(int x, int y, const std::string& text);
    void draw();
};
