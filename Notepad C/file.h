#pragma once
#include <windows.h>
#include <stdio.h>
#include "window.h"

typedef struct Window Window;

typedef struct File {
	WCHAR* fileName;
	HWND parentHwnd;
	WCHAR* fileContent;
	HANDLE hFile;
} File;

File* file(HWND hwnd);

void destroyFile(File* f);


WCHAR* getFileFromDialog(File* f, BOOL isOpenFileDialog);

WCHAR* getFileContent(File* f);

FILE* createFilePtr(File* f, wchar_t* mode);

void writeWCHARToFile(File* f, WCHAR* text);
void setFileName(File* f, WCHAR* name);
void saveFile(Window* w);
void saveFileAs(Window* w);
void openFile(Window* w);