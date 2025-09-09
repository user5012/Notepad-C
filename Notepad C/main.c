#include <windows.h>
#include "window.h"
#include "console.h"
#include <stdio.h>


#ifdef _DEBUG
int main(void)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int nCmdShow = SW_SHOW;
    Window* myWindow = win(hInstance, nCmdShow, 800, 600, L"OOP Window In C", L"MyWindowClass");
    if (!myWindow)
        return 1;
    printf("creating window...\n");
    
    printf("showing window...\n");
    show_window(myWindow);
    printf("Loop started!\n");
    MSG_Loop(myWindow);
    printf("destroying window\n");
    window_deconstruct(myWindow);
    return 0;
}
#endif

#ifdef NDEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Window* myWindow = win(hInstance, nCmdShow, 800, 600, L"OOP Window In C", L"MyWindowClass");
    if (!myWindow)
        return 1;
    printf("showing window...\n");
    show_window(myWindow);
    printf("Loop started!\n");
    MSG_Loop(myWindow);
    printf("destroying window\n");
    window_deconstruct(myWindow);
    return 0;
}
#endif
    
	
