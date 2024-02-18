#include <stdio.h>
#include <windows.h>
#include "texteditor.h"

int callback(DWORD key)
{
    char symbol = GetKeyFromUINT(key);
    printf("%c", symbol);
    return(0);
}

int main()
{
    activateKeyhook(&callback);
}

