#include "Platform.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <locale.h>
    #include <iostream>
#endif

void clearScreen() 
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifndef _WIN32
int getchWrapper() 
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#else
int getchWrapper() 
{
    return _getch();
}
#endif

void setupLocale() 
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#else
    setlocale(LC_ALL, "pl_PL.UTF-8");
    std::locale::global(std::locale("pl_PL.UTF-8"));
#endif
}


#ifndef _WIN32
#include <fcntl.h>
int kbhitWrapper() 
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) 
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
#endif
