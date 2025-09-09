#include "menu.h"
#include <stdio.h>

Menu* menu()
{
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (!menu) return NULL;
    menu->hMenu = CreateMenu();
    menu->hFileMenu = CreatePopupMenu();

    return menu;
}

void destroyMenu(Menu* m)
{
    if (!m) return;
	if (m->hMenu) DestroyMenu(m->hMenu);
    free(m);
}

void addMenuItems(Menu* m)
{
    AppendMenuW(m->hFileMenu, MF_STRING, 1, L"&Open");
    AppendMenuW(m->hFileMenu, MF_STRING, 2, L"&Save");
    AppendMenuW(m->hFileMenu, MF_STRING, 3, L"&Exit");

    AppendMenuW(m->hMenu, MF_POPUP, (UINT_PTR)m->hFileMenu, L"&File");
}

void attachMenu(Window* w)
{
    w->menu = menu();
    addMenuItems(w->menu);

    SetMenu(w->hwnd, w->menu->hMenu);
}

void handleMenu(Window* w, int wmId)
{
    switch (wmId) {
    case 1: { //Open
        /*MessageBoxW(w->hwnd, L"Open Pressed!", w->title, MB_OK);*/
        File* f = file(w->hwnd);
        WCHAR* fileName = getFileFromDialog(f);
		w->openedFileName = fileName;
        if (fileName) {
            WCHAR* fileContent = getFileContent(f);
            updateTxtBoxText(w->txt_boxes[0]->txtBox, fileContent);
        }

        destroyFile(f);
        break;
    }
    case 2: { //Save
		WCHAR* currentText = getTxtBoxText(w->txt_boxes[0]->txtBox);
        if (currentText) {

        }
        break;
    }
    case 3: { //Exit
        PostQuitMessage(0);
        break;
    }
    }
}


