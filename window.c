#include <windows.h>
#include "texteditor.h"
#include <winuser.h>
const char g_szClassName[] = "myWindowClass";
#define IDC_MAIN_EDIT 101
int (*hupdate)(HWND h);
char (*ptext)[1024];
int *tindex;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        LPRECT rect = malloc(sizeof(LPRECT) * 2);
        rect->bottom = ps.rcPaint.bottom;
        rect->left = ps.rcPaint.left;
        rect->right = ps.rcPaint.right;
        rect->top = ps.rcPaint.top;
        char test[1024];
        memcpy(test, ptext, 1024);
        int t = DrawTextExA(hdc, test, -1, rect, DT_LEFT | DT_TABSTOP | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL, NULL);


        test[*tindex] = '&';
        test[*tindex + 1] = 'p';
        DrawTextExA(hdc, test, -1, rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL | DT_PREFIXONLY, NULL);
        EndPaint(hwnd, &ps);
    }

    break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int InitializeWindow(LPSTR lpCmdLine, int nCmdShow, int* ti, char* textPointer[], int (*hc)(HWND h))
{
    //hupdate = hc;
    ptext = &textPointer[0];
    tindex = ti;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    // Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    // wc.hInstance = "Test";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "window registration failed", "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "TextEditor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, NULL, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "window creation failed", "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    hc(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}