#include "window.h"
#include <stdio.h>

HFONT hLabelFont = NULL;

Window* win(HINSTANCE hInstance, int nCmdShow, int width, int height, WCHAR* title, WCHAR* CLASSNAME, WCHAR* fileToOpen)
{
    Window* w = (Window*)calloc(1, sizeof(Window));
    if (!w)
        return NULL;
    if (fileToOpen) {
        w->fileToOpen = fileToOpen;
		printf("file to open: %ls\n", fileToOpen);
    }
    else
    {
		w->fileToOpen = NULL;
    }

    w->isSaved = TRUE;
	w->OpenedFilePtr = NULL;
	w->openedFileName = NULL;
	w->fileNameTitle = L"Untitled";
    w->width = width;
    printf("width set to: %d\n", width);
    w->height = height;
    printf("height set to: %d\n", height);
    w->title = title;
    w->fullTitle = merge_str(w->title, L" - ");
    printf("title: %ls\n", title);
	printf("full title: %ls\n", w->fullTitle);
    w->CLASSNAME = CLASSNAME;
    printf("classname: %ls\n", CLASSNAME);
    w->hInstance = hInstance;
    w->nCmdShow = nCmdShow;

    w->label_count = 0;
    w->button_count = 0;
    w->txt_boxes_count = 0;
    w->menu = NULL;
    addHaccelToWindow(w);
    printf("Keys added\n");


    w->hwnd = create_window(w);
	create_window_things(w);
	printf("hwnd: %p\n", w->hwnd);
    printf("Created window succesfully!\n");

    return w;
}

static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

        return 0;
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        Window* w = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)w);

        break;
    }
    case WM_COMMAND: {

        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);
        HWND hCtrl = (HWND)lParam;
        Window* w = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (w && w->button_count > 0)
            handle_buttons(wmId, w, wmEvent);
        if (w && w->menu)
            handleMenu(w, wmId);
        if (wmId == 3010) {
            updateTitleIfChanges(w);
        }

        printf("WM_COMMAND triggered: id=%d\n", wmId);

        break;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        return (INT_PTR)GetStockObject(NULL_BRUSH);
    }
    case WM_SIZE: {
        Window* w = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        printf("WM_SIZE: width=%d height=%d\n", width, height);
        if (w && w->txt_boxes_count > 0) {
            handleTxtBoxes(w);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_CLOSE: {
        Window* w = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (w) {
            closeWin(w);
        }
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND create_window(Window* w)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = w->hInstance;
    wc.lpszClassName = w->CLASSNAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

	w->fullTitle = merge_str(w->fullTitle, w->fileNameTitle);

    HWND hwnd = CreateWindowEx(
        0,
        w->CLASSNAME,
        w->fullTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w->width,
        w->height,
        NULL,
        NULL,
        w->hInstance,
        w);

    if (hwnd == NULL)
        return NULL;

    return hwnd;
}

void create_window_things(Window* w)
{
	if (!w) return;

    //menu
    attachMenu(w);
    printf("Menu attached\n");

    //keys
    

    //txt boxes
    createTxtBoxes(w);
    printf("Text boxes created\n");

    // labels
    //create_labels(w);

    ////buttons
    //create_buttons(w);

    // create fonts
    hLabelFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    // set font
    if (hLabelFont)
    {
        if (w && w->label_count > 0)
            for (int i = 0; i < w->label_count; i++) {
                set_font(w, w->labels[i]->id, hLabelFont);
            }
        if (w && w->button_count > 0)
            for (int i = 0; i < w->button_count; i++) {
                set_font(w, w->buttons[i]->id, hLabelFont);
            }
        if (w && w->txt_boxes_count > 0)
            for (int i = 0; i < w->txt_boxes_count; i++) {
                set_font(w, w->txt_boxes[i]->id, hLabelFont);
            }
    }

	// open file if specified
    if (w->fileToOpen) {
        openFile(w);
	}
	printf("Window things created\n");
}

WCHAR* merge_str(WCHAR* str1, WCHAR* str2)
{
    size_t baseLen = wcslen(str1);
    size_t fileLen = wcslen(str2);
    size_t totalLen = baseLen + fileLen + 1; // +1 for null terminator
    WCHAR* mergeStr = (WCHAR*)malloc(totalLen * sizeof(WCHAR));
    if (!mergeStr) return;
    wcscpy_s(mergeStr, totalLen, str1);
    wcscat_s(mergeStr, totalLen, str2);
	return mergeStr;
}

void show_window(Window* w)
{
    ShowWindow(w->hwnd, w->nCmdShow);
    UpdateWindow(w->hwnd);
}

void MSG_Loop(Window* w)
{
    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
		if (!TranslateAccelerator(GetAncestor(msg.hwnd, GA_ROOT), w->keys->hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }
}

void window_deconstruct(Window* w)
{
    if (!w) return;
    if (w && w->label_count > 0)
        destroy_labels(w);
    if (w && w->button_count > 0)
        destroy_buttons(w);
    if (w->menu) destroyMenu(w->menu);
    if (w && w->txt_boxes_count > 0)
        destroyTxtBoxes(w);
	if (w->openedFileName) free(w->openedFileName);
	if (w->OpenedFilePtr) destroyFile(w->OpenedFilePtr);
    if (hLabelFont)
    {
        DeleteObject(hLabelFont); // clear hlabelfont
    }
	if (w->keys) destroyKeys(w->keys);
    free(w);
}

void set_font(Window* w, int id, HFONT font)
{
    HWND hLabel = GetDlgItem(w->hwnd, id);
    if (hLabel && font)
    {
        SendMessageW(hLabel, WM_SETFONT, (WPARAM)font, TRUE);
    }
}

BOOL closeWinAsk(Window* w)
{
    WCHAR* currentText = getTxtBoxText(w->txt_boxes[0]->txtBox);
    if (!currentText) {
		free(currentText);
		return TRUE;
    }
    if (isFileSaved(w, currentText)) {
		free(currentText);
		return TRUE;
    }
    int result = MessageBoxW(w->hwnd, L"Do you want to save changes to your file?", w->title, MB_YESNOCANCEL | MB_ICONQUESTION);
	if (result == IDCANCEL) return FALSE;
    if (result == IDYES) {
		saveFile(w);
		return TRUE;
    }
    return TRUE;
}
void closeWin(Window* w)
{
    if (closeWinAsk(w)) {
		printf("Closing window...\n");
		PostQuitMessage(0);
	}
    else {
		printf("Close cancelled.\n");
        return;
    }
}

void updateTitleIfChanges(Window* w) {
    if (!w->isSaved) {
        
        if (w->OpenedFilePtr) {
            if (w->OpenedFilePtr->fileContent == getTxtBoxText(w->txt_boxes[0]->txtBox)) {
                SetWindowTextW(w->hwnd, removeWchar(w->fullTitle, L'*'));
                w->isSaved = TRUE;
                return;
            }
        }                                                                                           // TODO: NEED FIX
        else {
            if (!getTxtBoxText(w->txt_boxes[0]->txtBox)) {
                SetWindowTextW(w->hwnd, removeWchar(w->fullTitle, L'*'));
                w->isSaved = TRUE;
                return;
            }
        }
        if (isFileSaved(w, getTxtBoxText(w->txt_boxes[0]->txtBox))) {
            SetWindowTextW(w->hwnd, removeWchar(w->fullTitle, L'*'));
            w->isSaved = TRUE;
            return;
        }
    }
    else {
		SetWindowTextW(w->hwnd, merge_str(w->fullTitle, L"*"));
        w->isSaved = FALSE;
    }
}

WCHAR* removeWchar(WCHAR* str, WCHAR charToRemove) {
    if (!str) return NULL;

    WCHAR* src = str;
    WCHAR* dst = str;

    while (*src) {
        if (*src != charToRemove) {
            *dst++ = *src;  // copy only if not the char to remove
        }
        src++;
    }
    *dst = L'\0';  // null terminate

    return str;
}



BOOL isFileSaved(Window* w, WCHAR* currentText) {
	if (!w || !w->OpenedFilePtr) return FALSE;
	if (currentText == w->OpenedFilePtr->fileContent) {
		free(currentText);
		return TRUE;
	}
    else {
		free(currentText);
		return FALSE;
    }
}

void updateTitle(Window* w, WCHAR* newTitle)
{
    if (!w || !newTitle) return;
	w->fullTitle = merge_str(w->title, L" - "); // reset to base title
    w->fullTitle = merge_str(w->fullTitle, newTitle); // update the window's title field
    SetWindowTextW(w->hwnd, w->fullTitle);
}