#include <stdio.h>
#include <windows.h>
#define _WIN32_WINNT 0x050

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            DWORD keycode = p->vkCode;
            char symbol = (char)MapVirtualKeyA((u_int)keycode, (u_int)2);
            printf("%c", symbol);
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    // Install the low-level keyboard & mouse hooks
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

    // Keep this app running until we're told to stop
    MSG msg;
    while (!GetMessage(&msg, NULL, NULL, NULL))
    { // this while loop keeps the hook
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);

    return (0);
}
