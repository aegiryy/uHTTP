#include "cgi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#define BUF_SIZE 1024
#define PROG_NUM 3  
char* prog_suff[PROG_NUM]={"php","py","sh"};
char* prog_name[PROG_NUM]={"php-cgi","python","bash"};


void do_cgi(const char* path,const char* suffix,const char* params){
	FILE* fp;
	char data[BUF_SIZE];
	char cmd[BUF_SIZE];
	int i=0;
    memset(cmd, '\0', sizeof(cmd));
	int index=-1;
	for(i=0;i<PROG_NUM;i++){
		if(strcmp(prog_suff[i],suffix)==0){
			index=i;	
			break;		
		}
	}
	if(index==-1){
		printf("Wrong!");
		return;
	}
	strcpy(cmd, prog_name[index]);
	strcat(cmd, " ");
	strcat(cmd, path);
	strcat(cmd, " ");
	strcat(cmd, params);
	if((fp=popen(cmd,"r"))!=NULL){
        	while(fgets(data,sizeof(data),fp)!=NULL){
			printf("%s",data);
		}
		pclose(fp);
	}

    
}


