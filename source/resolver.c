#include <stdio.h>
#include <string.h>
#include "resolver.h"

static char * exts[] = {"php", "py"};

static int in_exts(char * ext);

int resolve(char * req, char * url, char * ext, char * params)
{
    char method[8], protocol[16];
    char * dot;
    char * quesmark;
    sscanf(req, "%[^ ] %[^ ] %[^ ]", method, url, protocol);
    dot = strrchr(url, '.');
    quesmark = strrchr(url, '?');
    if (dot == NULL)
        return 0;
    if (quesmark == NULL)
    {
        params = NULL;
        strcpy(ext, dot + 1);
    }
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
