#include "texteditor.h"
#include <windows.h>

#define _WIN32_WINNT 0x050

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam, void* callback(DWORD))
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            DWORD keycode = p->vkCode;
            callback(keycode);

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

void activateKeyhook(void* callback(DWORD))
{
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