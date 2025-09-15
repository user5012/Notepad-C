#include <windows.h>
#include "window.h"
#include "console.h"
#include <stdio.h>


static WCHAR* getFileToOpen(void) {
    int argc = 0;
	LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!wargv || argc < 2) return NULL;
	size_t len = wcslen(wargv[1]) + 1;
	WCHAR* copy = (WCHAR*)malloc(len * sizeof(WCHAR));
	if (copy) wcscpy_s(copy, len, wargv[1]);
	LocalFree(wargv);
	return copy;
}



#ifdef _DEBUG
int main(int argc, char* argv[])
{
	WCHAR* fileToOpen = getFileToOpen();
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int nCmdShow = SW_SHOW;
    Window* myWindow = win(hInstance, nCmdShow, 800, 600, L"OOP Window In C", L"MyWindowClass", fileToOpen);
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
    WCHAR* fileToOpen = getFileToOpen();

    Window* myWindow = win(hInstance, nCmdShow, 800, 600, L"OOP Window In C", L"MyWindowClass", fileToOpen);
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
    
	
