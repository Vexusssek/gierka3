#include "ConsoleBuffer.h"
#include <cstdio>

#ifdef _WIN32

ConsoleBuffer::ConsoleBuffer(int w, int h) : width(w), height(h) 
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    bufferSize.X = width;
    bufferSize.Y = height;
    
    screenBuffer = new CHAR_INFO[width * height];
    
    writeRegion = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
    
    clear();
}

ConsoleBuffer::~ConsoleBuffer() 
{
    delete[] screenBuffer;
}

void ConsoleBuffer::clear() 
{
    for (int i = 0; i < width * height; i++) 
    {
        screenBuffer[i].Char.AsciiChar = ' ';
        screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

void ConsoleBuffer::write(int x, int y, const std::string& text) 
{
    if (y < 0 || y >= height || x < 0 || x >= width) return;
    for (size_t i = 0; i < text.length() && (x + i) < static_cast<size_t>(width); i++) 
    {
        int index = y * width + x + i;
        screenBuffer[index].Char.AsciiChar = text[i];
        screenBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

void ConsoleBuffer::draw() 
{
    WriteConsoleOutput
    (
        hConsole,
        screenBuffer,
        bufferSize,
        {0, 0},
        &writeRegion
    );
}

#else

// IMPLEMENTACJA DLA LINUXA

#include <iostream>
#include <sstream>
#include <cstdlib>

ConsoleBuffer::ConsoleBuffer(int w, int h) : width(w), height(h) 
{
    screenBuffer.resize(height, std::string(width, ' '));
}

ConsoleBuffer::~ConsoleBuffer() 
{
    //tu nic
}

void ConsoleBuffer::clear() 
{
    for (int i = 0; i < height; i++) 
    {
        screenBuffer[i] = std::string(width, ' ');
    }
}

void ConsoleBuffer::write(int x, int y, const std::string& text) 
{
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    for (size_t i = 0; i < text.length() && (x + i) < static_cast<size_t>(width); i++) 
    {
        screenBuffer[y][x + i] = text[i];
    }
}

void ConsoleBuffer::draw() 
{
    std::cout << "\033[2J\033[H";
    for (int i = 0; i < height; i++) 
    {
        std::cout << screenBuffer[i] << "\n";
    }
    std::cout.flush();
}

#endif
