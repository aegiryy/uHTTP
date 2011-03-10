#include "control.h"
#include <stdlib.h>
#include <string.h>

char * respond(char * rawreq)
{
    char msg[] = "HTTP/1.1 200 OK\nDate: Thu, 10 Mar 2011 15:31:59 GMT\nContent-Type: text/html;charset=ISO-8859-1\nContent-Length: 13\n\nHello, world!";
    char * resp = (char *)malloc(strlen(msg) + 1);
    strcpy(resp, msg);
    return resp;
}
