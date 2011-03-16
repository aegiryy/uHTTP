#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "static.h"
#include "resolver.h"


char * static_serve(char * rawreq, unsigned int * size)
{
    char * data;
    struct stat stbuf;
    FILE * fp;
    char path[256] = ".";
    char url[255];
    rslv_get_url(rawreq, url);
    strcat(path, url);
    fp = fopen(path, "r");
    if (fp == NULL)
        return NULL;
    stat(path, &stbuf);
    data = (char *)malloc((unsigned int)stbuf.st_size + 1);
    //*size = (unsigned int)stbuf.st_size + 1;
    fread(data, 1, stbuf.st_size, fp);
    data[stbuf.st_size] = '\0';
    fclose(fp);
    return data;
}
