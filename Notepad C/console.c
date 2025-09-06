#include "console.h"
#include <windows.h>
#include <stdio.h>

void showConsole()
{
    AllocConsole();

    FILE* fp_stdout;
    FILE* fp_stdin;
    FILE* fp_stderr;

    freopen_s(&fp_stdout,"CONOUT$", "w", stdout);
    freopen_s(&fp_stdin, "CONIN$", "r", stdin);
    freopen_s(&fp_stderr, "CONOUT$", "w", stderr);

    SetConsoleTitle(L"Debug Console");
}