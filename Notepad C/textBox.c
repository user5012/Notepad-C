#include "textBox.h"

TextBox* txtBox(WCHAR* defaultText, int x, int y, int width, int height, HWND parentHwnd, HINSTANCE hInstance, int id)
{
	TextBox* t = (TextBox*)malloc(sizeof(TextBox));
	if (!t) return NULL;

	size_t len = wcslen(defaultText) + 1;
	t->defaultText = (WCHAR*)malloc(len * sizeof(WCHAR));
	if (t->defaultText) wcscpy_s(t->defaultText, len, defaultText);

	t->x = x;
	t->y = y;
	t->width = width;
	t->height = height;
	t->parentHwnd = parentHwnd;
	t->hInstance = hInstance;
	t->id = id;
	t->txtBox = createTxtBox(t);

	return t;
}

void destroyTxtBox(TextBox* t)
{
	if (!t) return;
	if (t->txtBox) DestroyWindow(t->txtBox);
	if (t->defaultText) free(t->defaultText);
	free(t);
}

HWND createTxtBox(TextBox* t)
{
	return CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		t->defaultText,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | WS_VSCROLL,
		t->x, t->y, t->width, t->height,
		t->parentHwnd,
		(HMENU)(INT_PTR)t->id,
		t->hInstance,
		NULL
	);
}

void createTxtBoxes(Window* w)
{

	int txtBoxes_count = 1;
	w->txt_boxes_count = txtBoxes_count;

	RECT rc;
	GetClientRect(w->hwnd, &rc);

	w->txt_boxes = (TextBox**)calloc(txtBoxes_count, sizeof(TextBox*));
	if (!w->txt_boxes) return;

	w->txt_boxes[0] = txtBox(L"", 0,0,rc.right - rc.left,rc.bottom - rc.top,w->hwnd, w->hInstance, 3010);
}

void destroyTxtBoxes(Window* w)
{
	for (int i = 0; i < w->txt_boxes_count; i++) {
		destroyTxtBox(w->txt_boxes[i]);
	}

	free(w->txt_boxes);
	w->txt_boxes = NULL;
	w->txt_boxes_count = 0;
}


void handleTxtBoxes(Window* w)
{
	if (w && w->txt_boxes_count > 0 && w->txt_boxes[0]) {
		RECT rc;
		GetClientRect(w->hwnd, &rc);
		MoveWindow(w->txt_boxes[0]->txtBox, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	}
}

void updateTxtBoxText(HWND txtBox, WCHAR* newText)
{
	SetWindowTextW(txtBox, newText);
}
