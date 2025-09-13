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
    AppendMenuW(m->hFileMenu, MF_STRING, 0, L"&Open\tCtrl+O");
    AppendMenuW(m->hFileMenu, MF_STRING, 1, L"&Save\tCtrl+S");
	AppendMenuW(m->hFileMenu, MF_STRING, 2, L"&Save As\tCtrl+Alt+S");
	AppendMenuW(m->hFileMenu, MF_SEPARATOR, 3, NULL);
    AppendMenuW(m->hFileMenu, MF_STRING, 4, L"&Exit");

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
    case 0: { //Open
        /*MessageBoxW(w->hwnd, L"Open Pressed!", w->title, MB_OK);*/
        File* f = file(w->hwnd);
        WCHAR* fileName = getFileFromDialog(f, TRUE);
		w->openedFileName = fileName;
		updateTitle(w, w->openedFileName);
        if (fileName) {
            WCHAR* fileContent = getFileContent(f);
            updateTxtBoxText(w->txt_boxes[0]->txtBox, fileContent);
			w->OpenedFilePtr = f;
        }
		else {
			perror("No file selected");
            destroyFile(f);
		}
        
        break;
    }
    case 1: { //Save
		saveFile(w);
        
        break;
    }
    case 2: { //Save As
		saveFileAs(w);
		break;
    }
    case 4: { //Exit
		closeWin(w);
        break;
    }
    }
}


