#include <windows.h>

//keyboard.c
void activateKeyhook(int (*callback)(DWORD, char));
char GetKeyFromUINT(UINT key);

