#pragma once
#include <windows.h>
#include <stdio.h>



typedef struct File {
	WCHAR* fileName;
	HWND parentHwnd;
	WCHAR* fileContent;
	FILE* fptr;
} File;

typedef enum {
	ENC_UTF8,
	ENC_UTF16LE,
	ENC_UTF16BE,
	ENC_ANSI
} Encoding;

File* file(HWND hwnd);

void destroyFile(File* f);


WCHAR* getFileFromDialog(File* f);

WCHAR* getFileContent(File* f);

FILE* createFilePtr(File* f, wchar_t* mode);

WCHAR* readTextFileToWCHAR(FILE* f, Encoding enc, size_t* outLength);

void printUtf16(WCHAR* text, File* f);