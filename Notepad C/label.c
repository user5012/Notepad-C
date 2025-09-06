#include "label.h"

Label* label(Window* w, int x, int y, int width, int height, WCHAR* text, int id)
{
    Label* lbl = (Label*)malloc(sizeof(Label));
    if (!lbl) return NULL;


    lbl->w = w;
    lbl->x = x;
    lbl->y = y;
    lbl->width = width;
    lbl->height = height;
    lbl->id = id;
    
    size_t len = wcslen(text) + 1;
    lbl->text = (WCHAR*)malloc(len * sizeof(WCHAR));
    if (lbl->text) wcscpy_s(lbl->text, len, text);

    lbl->label = create_label(lbl);

    return lbl;
}

void destroy_label(Label* lbl)
{
    if (!lbl) return;
    if (lbl->label) DestroyWindow(lbl->label);
    if (lbl->text) free(lbl->text);

    free(lbl);
}

HWND create_label(Label* lbl)
{
    return CreateWindowW (
        L"STATIC",
        lbl->text,
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        lbl->x, lbl->y,
        lbl->width,
        lbl->height,
        lbl->w->hwnd,
        (HMENU)(INT_PTR)lbl->id,
        lbl->w->hInstance,
        NULL
    );
}

void set_label_text(Label* lbl, WCHAR* newText)
{
    HWND hlabel = GetDlgItem(lbl->w->hwnd, lbl->id);
    if (hlabel)
    {
        SetWindowText(hlabel, newText);
    }
}

void create_labels(Window* w)
{
    
    int num_labels = 1;
    w->labels = (Label**)malloc(num_labels * sizeof(Label*));
    w->label_count = num_labels;

    w->labels = (Label**)calloc(num_labels, sizeof(Label*));
    if (!w->labels) return;

    w->labels[0] = label(w, 100, 100, 100, 30, L"I am a label", 1010);
}

void destroy_labels(Window* w)
{
    if (!w->labels) return;
    for (int i = 0; i < w->label_count; i++) {
        destroy_label(w->labels[i]);
    }
    free(w->labels);
    
    w->labels = NULL;
    w->label_count = 0;
}

