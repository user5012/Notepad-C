#include "window.h"
#include <stdio.h>

HFONT hLabelFont = NULL;

Window* win(HINSTANCE hInstance, int nCmdShow, int width, int height, WCHAR* title, WCHAR* CLASSNAME)
{
    Window* w = (Window*)malloc(sizeof(Window));
    if (!w)
        return NULL;

    w->width = width;
    printf("width set to: %d\n", width);
    w->height = height;
    printf("height set to: %d\n", height);
    w->title = title;
    printf("title: %ls\n", title);
    w->CLASSNAME = CLASSNAME;
    printf("classname: %ls\n", CLASSNAME);
    w->hInstance = hInstance;
    w->nCmdShow = nCmdShow;
    w->hwnd = create_window(w);
    printf("Created window succesfully!\n");

    return w;
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        if (hLabelFont)
        {
            DeleteObject(hLabelFont); // clear hlabelfont
        }
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        Window* w = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)w);

        //menu
        attachMenu(w);

        //txt boxes
        createTxtBoxes(w);

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

        break;
    }
    case WM_COMMAND: {
        
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);
        HWND hCtrl = (HWND)lParam;
        Window* w = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (w && w->button_count> 0)
            handle_buttons(wmId, w, wmEvent);
        if (w && w->menu)
            handleMenu(w, wmId);
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
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND create_window(Window* w)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc; // add later
    wc.hInstance = w->hInstance;
    wc.lpszClassName = w->CLASSNAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        w->CLASSNAME,
        w->title,
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
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
