#include <stdio.h>
#include <windows.h>
#include "texteditor.h"
#include <stdbool.h>

char text[1024];
int textIndex = 0;

int AppendText(char c)
{
    switch (c)
    {
    case 10:

        text[textIndex] = '\n';
        textIndex++;
        return textIndex;

    case 8:
        if(textIndex == 0)
            return textIndex;
        textIndex--;
        text[textIndex] = ' ';
        return textIndex;

    default:
        text[textIndex] = c;
        textIndex++;
        return textIndex;
    }
}
void Save()
{
    FILE *textfile = fopen("C:\\Programming\\PROJECTS\\C\\TextEditor\\textfile.txt", "w");
    fprintf(textfile, text);
}

int callback(DWORD key, char c)
{
    switch (key)
    {
    default:

        if(c == NULL)
            return(0);

        printf("%c", c);
        AppendText(c);
        return (0);

    case VK_BACK:
        printf("\b \b");
        AppendText(8);
        return (0);

    case VK_RETURN:
        printf("\n");
        AppendText('\n');
        return (0);

    case VK_ESCAPE:
        Save();
        exit(0);
    }
}

void ActivateWindow()
{
    InitializeWindow(GetCommandLine(), 5);
}

int main()
{
    ActivateWindow();
    activateKeyhook(&callback);
}
