#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server.h"
#include "static.h"
#include "resolver.h"


// static void geturl(char * rawreq, char * url);

char * respond(char * rawreq)
{
    char postfix[8];
    get_postfix(rawreq, postfix);
    return staticserve(rawreq);
}

