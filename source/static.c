#include "static.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char * render(char * path)
{
    char * data;
    struct stat stbuf;
    FILE * fp;
    stat(path, &stbuf);
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        data = (char *)malloc(11);
        data = "Not Found!";
        return data;
    }
    data = (char *)malloc((unsigned int)stbuf.st_size + 1);
    fread(data, 1, stbuf.st_size, fp);
    data[stbuf.st_size] = '\0';
    return data;
}
