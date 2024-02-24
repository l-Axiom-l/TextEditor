#include <windows.h>
#include <stdbool.h>

//keyboard.c
void activateKeyhook(int (*callback)(DWORD, char), bool* iS);
char GetKeyFromUINT(UINT key);

//window.c
int InitializeWindow(LPSTR lpCmdLine, int nCmdShow, int* ti, char* textPointer[], int (*hc)(HWND h));