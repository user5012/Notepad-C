#include <windows.h>
#include "window.h"
#include "console.h"
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    showConsole();
#endif

#ifdef NDEBUG
    
#endif
    

    Window* myWindow = win(hInstance, nCmdShow, 800, 600, L"OOP Window In C", L"MyWindowClass");
    if (!myWindow)
        return 1;
    printf("creating window...\n");
    create_window(myWindow);
    printf("showing window...\n");
    show_window(myWindow);
    printf("Loop started!\n");
    MSG_Loop(myWindow);
    printf("destroying window\n");
    window_deconstruct(myWindow);
    return 0;
}