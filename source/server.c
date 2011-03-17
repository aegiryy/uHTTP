#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"
#include "static.h"
#include "resolver.h"

static char * mapping[] = {
    "html", "text/html; charset=UTF-8",
    "js", "text/javascript",
    "png", "image/png", 
    "jpg", "image/jpeg",
    "css", "text/css",
    "", "text/plain"
};

static int sz_mapping = sizeof(mapping) / sizeof(mapping[0]) / 2;

static int get_contenttype(char * ext);

char * respond(char * rawreq)
{
    char ext[8];
    char * content;
    char * resp;
    unsigned int ctsize;
    rslv_get_ext(rawreq, ext);
    content = static_serve(rawreq, &ctsize);
    if (content == NULL)
    {
        char status[] = "HTTP/1.0 404 Not Found";
        content = (char *)malloc(strlen(status) + 1);
        strcpy(content, status);
        return content;
    }
    else
    {
        char status[] = "HTTP/1.0 200 OK\n";
        char header[] = "Content-Type: ";
        char * type;
        char header_end[] = "\nLast-Modified: Thu, 05 Aug 2010 22:54:44 GMT\nDate: Wed, 16 Mar 2011 11:35:28 GMT\nConnection: close\n\n";
        type = mapping[2 * get_contenttype(ext) + 1];
        resp = (char *)malloc(strlen(status) + strlen(header) + strlen(type) + strlen(header_end) + ctsize);
        resp[0] = '\0';
        strcat(resp, status);
        strcat(resp, header);
        strcat(resp, type);
        strcat(resp, header_end);
        printf("%s\n", resp);
        strcat(resp, content);
        free(content);
        return resp;
    }
}

static int get_contenttype(char * ext)
{
    int i;
    for (i = 0; i < sz_mapping; i++)
        if (strcmp(mapping[i * 2], ext) == 0)
            return i;
    return -1;
}
