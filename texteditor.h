#include <windows.h>

//keyboard.c
void activateKeyhook(int (*callback)(DWORD, char));
char GetKeyFromUINT(UINT key);

//window.c
int InitializeWindow(LPSTR lpCmdLine, int nCmdShow);
