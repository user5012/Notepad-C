#pragma once
#include <windows.h>
#include "button.h"
#include "label.h"
#include "menu.h"
#include "textBox.h"
#include "file.h"
#include "keys.h"

#define ID_LABEL_1 1010
#define ID_BUTTON_1 2010

typedef struct Button Button;

typedef struct Label Label;

typedef struct Menu Menu;

typedef struct TextBox TextBox;

typedef struct File File;
typedef struct Keys Keys;
typedef struct Window
{
    WCHAR* CLASSNAME;
    WCHAR* title;
	WCHAR* fullTitle;
	WCHAR* fileNameTitle;
    int width;
    int height;
    HINSTANCE hInstance;
    HWND hwnd;
    int nCmdShow;
    Button** buttons;
    int button_count;
    Label** labels;
    int label_count;
    Menu* menu;
    TextBox** txt_boxes;
    int txt_boxes_count;
    WCHAR* openedFileName;
    File* OpenedFilePtr;
	Keys* keys;
    BOOL isSaved;
} Window;

Window* win(HINSTANCE hInstance, int nCmdShow, int width, int height, WCHAR* title, WCHAR* CLASSNAME);

HWND create_window(Window* w);

void create_window_things(Window* w);

void show_window(Window* w);

void MSG_Loop(Window* w);

void window_deconstruct(Window* w);

void set_font(Window* w, int id, HFONT font);

void closeWin(Window* w);
BOOL closeWinAsk(Window* w);

BOOL isFileSaved(Window* w, WCHAR* currentText);

WCHAR* merge_str(WCHAR* str1, WCHAR* str2);
void updateTitle(Window* w, WCHAR* newTitle);

void updateTitleIfChanges(Window* w);
WCHAR* removeWchar(WCHAR* str, WCHAR charToRemove);