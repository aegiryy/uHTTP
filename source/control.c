#include "control.h"
#include "static.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void geturl(char * rawreq, char * url);

char * respond(char * rawreq)
{
    char url[256];
    char rootdir[256] = ".";
    geturl(rawreq, url);
    return render(strcat(rootdir, url));
}

static void geturl(char * rawreq, char * url)
{
    int i = 4;
    while (rawreq[i] != ' ')
    {
        url[i - 4] = rawreq[i];
        i++;
    }
    url[i - 4] = '\0';
}
