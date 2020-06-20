#include "utils.h"

void spaceString(char *_Str, char *_Dest)
{
    char *finalstr;
    finalstr = (char*)malloc(strlen(_Str) * 2 + 1);
    memset(finalstr, 0, sizeof(finalstr));

    char space = ' ';

    for (int s1 = 0; s1 < strlen(_Str); s1 += 1)
    {

        strncat(finalstr, &_Str[s1], 1);
        strncat(finalstr, &space, 1);
    }

    strcpy(_Dest, finalstr);
}

void clearConsole() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

}

char toLowerCase(char c)
{
    const char LOWER[] = "abcdefghijklmnopqrstuvwxyz";
    const char UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < strlen(UPPER); i++)
        if (c == UPPER[i]) 
            return LOWER[i];

    return c;
}