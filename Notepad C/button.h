#pragma once
#include <windows.h>
#include "window.h"

typedef struct Window Window;

typedef struct Button {
	WCHAR* text;
	int x;
	int y;
	int width;
	int height;
	Window* ParentHwnd;
	int id;
	HWND button;
} Button;

Button* btn(Window* w, int x, int y, int width, int height, WCHAR* text, int id);
void destroy_btn(Button* b);

HWND create_button(Button* b);
void create_buttons(Window* w);
void destroy_buttons(Window* w);

void handle_buttons(int wmId, Window* w, int wmEvent);