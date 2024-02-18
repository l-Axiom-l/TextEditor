#include "texteditor.h"
#include <Windows.h>

int (*callbackLocal)(DWORD d, char c) = NULL;

struct unicodethings
{
    UINT vk;
    UINT sc;
    char c;
};

void GetUnicode(struct unicodethings u)
{
    byte keyboardstate[256];
    WCHAR buf[256];
    GetKeyboardState(&keyboardstate);
    unsigned int shift = GetAsyncKeyState(VK_UP);
    shift = shift >> 15;

    if ((shift) == 1)
        keyboardstate[VK_LSHIFT] |= 0x80;
    UINT wflags = 0b10;
    int test = ToUnicodeEx(u.vk, u.sc, keyboardstate, buf, 256, wflags, GetKeyboardLayout(0));
    u.c = buf[0];
    ExitThread(0);
    // if (test > 0)
    //     return buf[0];
    // else
    //     return NULL;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
        case WM_KEYDOWN:
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            DWORD keycode = p->vkCode;
            if (p->vkCode == VK_LSHIFT)
                break;

            struct unicodethings ut;
            ut.vk = p->vkCode;
            ut.sc = p->scanCode;
            HANDLE h = CreateThread(NULL, 0, &GetUnicode, &ut, 0, NULL);
            WaitForSingleObject(h, 100);
            CloseHandle(h);
            
            callbackLocal(keycode, ut.c);

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

void activateKeyhook(int (*callback)(DWORD, char))
{
    callbackLocal = callback;
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    UnhookWindowsHookEx(hhkLowLevelKybd);
}