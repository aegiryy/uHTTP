#include <stdio.h>
#include <string.h>
#include "resolver.h"

void rslv_get_url(char * rawreq, char * url)
{
    int i, j;
    for (i = 0; rawreq[i] != ' '; i++);
    i++;
    for (j = 0; rawreq[i] != ' '; j++, i++)
        url[j] = rawreq[i];
    url[j] = '\0';
}

void rslv_get_method(char * rawreq, char * method)
{
    int i;
    for (i = 0; rawreq[i] != ' '; i++)
        method[i] = rawreq[i];
    method[i] = '\0';
}

void rslv_get_postfix(char * rawreq, char * postfix)
{
    int i, j;
    char url[256];
    rslv_get_url(rawreq, url);
    for (i = 0; url[i] != '\0'; i++);
    for (; url[i] != '.' && url[i] != '/'; i--);
    if (url[i] == '/')
    {
        postfix[0] = '\0';
        return;
    }
    i++;
    for (j = 0; url[i] != '\0'; i++, j++)
        postfix[j] = url[i];
    postfix[j] = '\0';
}
