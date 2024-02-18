#include <stdio.h>
#include <windows.h>
#include "texteditor.h"
#define _WIN32_WINNT 0x050

int main()
{
    activateKeyhook(&callback);
}

void callback(DWORD key)
{
    char symbol = GetKeyFromUINT(key);
    printf("%c", symbol);
}