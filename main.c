#include <stdio.h>
#include <windows.h>
#include "texteditor.h"
#include <stdbool.h>

char text[1024];
int textIndex = 0;
int textLenght = 0;
HWND thisWindow = NULL;
bool isSuspended = FALSE;
bool inserting = FALSE;

int AppendText(char c)
{
    switch (c)
    {
    case 10:
        text[textIndex] = '\n';
        textIndex++;
        textLenght++;
        return textIndex;

    case 8:
        if (textIndex == 0)
            return textIndex;
        textIndex--;
        textLenght--;
        text[textIndex] = ' ';
        return textIndex;

    default:
        text[textIndex] = c;
        textIndex++;
        textLenght++;
        return textIndex;
    }
}

int InsertText(char c)
{
    switch (c)
    {
    case 10:
        memcpy(&text[textIndex + 1], &text[textIndex], 1024 - textIndex - 1);
        text[textIndex] = '\n';
        textIndex++;
        textLenght++;
        return textIndex;

    case 8:
        if (textIndex == 0)
            return textIndex;

        memcpy(&text[textIndex - 1], &text[textIndex], 1024 - textIndex);
        textIndex--;
        textLenght--;

        return textIndex;

    default:
        memcpy(&text[textIndex + 1], &text[textIndex], 1024 - textIndex - 1);
        text[textIndex] = c;
        textIndex++;
        textLenght++;
        return textIndex;
    }
}

void Save()
{
    OPENFILENAME ofn = {0};
    char szFile[1024];
    memcpy(szFile, "File.txt", 9);
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "*.txt";
    ofn.nFilterIndex = 0;
    ofn.lpstrInitialDir = "Documents";
    ofn.Flags = OFN_EXPLORER;

    if (GetSaveFileNameA(&ofn) == TRUE)
    {
        FILE *fi = fopen(ofn.lpstrFile, "w");
        fprintf(fi, text);
        fclose(fi);
    }
}

int callback(DWORD key, char c)
{
    switch (key)
    {
    default:

        if (c == NULL)
            return (0);

        printf("%c", c);
        if (inserting)
            InsertText(c);
        else
            AppendText(c);
        RedrawWindow(thisWindow, NULL, NULL, RDW_INVALIDATE);
        return (0);

    case VK_BACK:
        printf("\b \b");
        if (inserting)
            InsertText(8);
        else
            AppendText(8);
        RedrawWindow(thisWindow, NULL, NULL, RDW_INVALIDATE);
        return (0);

    case VK_RETURN:
        printf("\n");
        if (inserting)
            InsertText('\n');
        else
            AppendText('\n');
        RedrawWindow(thisWindow, NULL, NULL, RDW_INVALIDATE);
        return (0);

    case VK_LEFT:
        if (textIndex > 0)
        {
            textIndex--;
            RedrawWindow(thisWindow, NULL, NULL, RDW_INVALIDATE);
        }
        return (0);

    case VK_RIGHT:
        if (textIndex < textLenght)
        {
            textIndex++;
            RedrawWindow(thisWindow, NULL, NULL, RDW_INVALIDATE);
        }
        return (0);

    case VK_ESCAPE:
        Save();
        exit(0);
    }
}

int handleCallback(HWND h)
{
    thisWindow = h;
    return (0);
}

int ActivateWindow()
{
    InitializeWindow(GetCommandLine(), 5, &textIndex, &text, &handleCallback);
    return (0);
}

int UPDATE()
{
    while (1)
    {
        isSuspended = GetForegroundWindow() == thisWindow ? FALSE : TRUE;
        inserting = textIndex == textLenght ? FALSE : TRUE;
        Sleep(100);
    }

    return (0);
}

int main()
{
    HANDLE handle = CreateThread(NULL, 0, ActivateWindow, NULL, 0, NULL);
    HANDLE ac = CreateThread(NULL, 0, UPDATE, NULL, 0, NULL);
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    activateKeyhook(&callback, &isSuspended);
}
