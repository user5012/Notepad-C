#pragma once
#include <windows.h>
#include <stdio.h>



typedef struct File {
	WCHAR* fileName;
	HWND parentHwnd;
	WCHAR* fileContent;
	HANDLE hFile;
} File;

File* file(HWND hwnd);

void destroyFile(File* f);


WCHAR* getFileFromDialog(File* f);

WCHAR* getFileContent(File* f);

FILE* createFilePtr(File* f, wchar_t* mode);

void writeWCHARToFile(File* f, WCHAR* text);
void setFileName(File* f, WCHAR* name);