#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"
#include "static.h"
#include "resolver.h"


char * respond(char * rawreq)
{
    char postfix[8];
    char * content;
    char * resp;
    unsigned int ctsize;
    rslv_get_postfix(rawreq, postfix);
    content = static_serve(rawreq, &ctsize);
    if (content == NULL)
    {
        char status[] = "HTTP/1.1 404 Not Found";
        content = (char *)malloc(strlen(status) + 1);
        strcpy(content, status);
        return content;
    }
    else
    {
        char status[] = "HTTP/1.1 200 OK\n";
        char header[] = "Content-Type: text/html; charset=UTF-8\n\n";
        resp = (char *)malloc(strlen(status) + strlen(header) + ctsize);
        resp[0] = '\0';
        strcat(resp, status);
        strcat(resp, header);
        strcat(resp, content);
        return resp;
    }
}

