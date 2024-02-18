#include "texteditor.h"
#include <windows.h>

int (*callbackLocal)(DWORD d);

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            DWORD keycode = p->vkCode;
            callbackLocal(keycode);

        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

char GetKeyFromUINT(UINT key)
{
    return (char)MapVirtualKeyA((u_int)key, (u_int)2);
}

void activateKeyhook(int (*callback)(DWORD))
{
    callbackLocal = callback;
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

    // Keep this app running until we're told to stop
    MSG msg;
    while (!GetMessage(&msg, NULL, NULL, NULL))
    { // this while loop keeps the hook
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
}