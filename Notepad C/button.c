#include "button.h"

#define ID_BUTTON_1 2010

Button* btn(Window* w, int x, int y, int width, int height, WCHAR* text, int id)
{
	Button* b = (Button*)malloc(sizeof(Button));

	if (!b) return NULL;

	b->ParentHwnd = w;
	b->x = x;
	b->y = y;
	b->width = width;
	b->height = height;
	b->id = id;
	

	size_t len = wcslen(text) + 1;
	b->text = (WCHAR*)malloc(len * sizeof(WCHAR));
	if (b->text) wcscpy_s(b->text, len, text);

	b->button = create_button(b);

	return b;
}

void destroy_btn(Button* b)
{
	if (!b) return;

	if (b->button) DestroyWindow(b->button);
	if (b->text) free(b->text);
	free(b);
}

HWND create_button(Button* b)
{
	return CreateWindowW(
		L"BUTTON",
		b->text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		b->x, b->y,
		b->width,
		b->height,
		b->ParentHwnd->hwnd,
		(HMENU)(INT_PTR)b->id,
		b->ParentHwnd->hInstance,
		NULL
	);
}

void destroy_buttons(Window* w)
{
	if (!w->buttons) return;

	for (int i = 0; i < w->button_count; i++) {
		destroy_btn(w->buttons[i]);
	}
	free(w->buttons);
	
	w->buttons = NULL;
	w->button_count = 0;
}

void handle_buttons(int wmId, Window* w, int wmEvent)
{
	if (!w->buttons) return;
	if (wmId == w->buttons[0]->id && wmEvent == BN_CLICKED) {

		if (w)
		{
			MessageBoxW(w->hwnd, L"Button Clicked!", w->title, MB_OK);
			printf("Button Clicked!\n");
		}
	}
}

void create_buttons(Window* w)
{
	if (!w) return;

	int num_buttons = 1;
	w->buttons = (Button**)malloc(num_buttons * sizeof(Button*));
	w->button_count = num_buttons;

	w->buttons = (Label**)calloc(num_buttons, sizeof(Label*));
	if (!w->buttons) return;

	w->buttons[0] = btn(w, 50, 50, 100, 50, L"Click me", 2010);
	
}

