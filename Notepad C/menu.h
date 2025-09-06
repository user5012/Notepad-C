#pragma once
#include <windows.h>
#include "window.h"
#include "file.h"


typedef struct Window Window;

typedef struct Menu {
	HMENU hMenu;
	HMENU hFileMenu;
} Menu;

Menu* menu();
void destroyMenu(Menu* m);

void addMenuItems(Menu* m);

void attachMenu(Window* w);

void handleMenu(Window* w, int wmId);