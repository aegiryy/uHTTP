#ifndef _CONTROL_H
#define _CONTROL_H
#define BUFFER_SIZE         1024 

/* return value should be freed by caller */
extern char req[BUFFER_SIZE];

char * respond(char * rawreq);

#endif
