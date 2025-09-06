#pragma once
#include <windows.h>
#include "window.h"

typedef struct Window Window;

typedef struct Label {
	Window* w; 
	int x;
	int y;
	int width;
	int height;
	WCHAR* text;
	int id;
	HWND label;
} Label;

Label* label(Window* w, int x, int y, int width, int height, WCHAR* text, int id);
void destroy_label(Label* lbl);

HWND create_label(Label* lbl);

void set_label_text(Label* lbl, WCHAR* newText);

void create_labels(Window* w);
void destroy_labels(Window* w);