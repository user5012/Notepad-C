#pragma once
#include <windows.h>
#include "window.h"

typedef struct Window Window;

typedef struct TextBox {
	WCHAR* defaultText;
	int x;
	int y;
	int width;
	int height;
	int parentHwnd;
	HINSTANCE hInstance;
	HWND txtBox;
	int id;
} TextBox;

TextBox* txtBox(WCHAR* defaultText, int x, int y, int width, int height, HWND parentHwnd, HINSTANCE hInstance, int id);

void destroyTxtBox(TextBox* t);

HWND createTxtBox(TextBox* t);

void createTxtBoxes(Window* w);
void destroyTxtBoxes(Window* w);

void handleTxtBoxes(Window* w);