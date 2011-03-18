#include <stdio.h>
#include <string.h>
#include "resolver.h"

static char * exts[] = {"php", "py", "sh"};

static int in_exts(char * ext);

int resolve(char * url, char * ext, char * params)
{
    char * dot;
    char * quesmark;
    dot = strrchr(url, '.');
    quesmark = strrchr(url, '?');
    if (dot == NULL)
        return 0;
    if (quesmark == NULL)
        strcpy(ext, dot + 1);
    else
    {
        char * andmark;
        *quesmark = '\0';
        strcpy(params, quesmark + 1);
        while ((andmark = strrchr(params, '&')) != NULL)
            *andmark = ' ';
        strcpy(ext, dot + 1);
    }
    return in_exts(ext);
}

int in_exts(char * ext)
{
    int i;
    for (i = 0; i < sizeof(exts) / sizeof(exts[0]); i++)
        if (strcmp(ext, exts[i]) == 0)
            return 1;
    return 0;
}
