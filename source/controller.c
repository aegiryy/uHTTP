#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include "resolver.h"
#include "cgi.h"

#define SERVER_NAME "uHTTP"
#define SERVER_URL "https://github.com/aegiryy/uHTTP"
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"


/* Forwards. */
static void file_details(char* dir, char* name);
static void send_error(int status, char* title, char* extra_header, char* text);
static void send_headers(int status, char* title, char* extra_header, char* mime_type, off_t length, time_t mod);
static char* get_mime_type(char* name);
static void strdecode(char* to, char* from);
static int hexit(char c);
static void strencode(char* to, size_t tosize, const char* from);

#ifdef XINETD
int main(int argc, char * argv[])
#else
int serve(char * rootdir)
#endif
{
    char line[10000], method[8], path[1024], protocol[16], idx[20000], location[20000];
    char* file;
    size_t len;
    int ich;
    struct stat sb;
    FILE* fp;
    struct dirent **dl;
    int i, n;
    int result;
    char filepath[1024], ext[8], params[128];
#ifdef XINETD
    char * rootdir;
    if (argc < 2)
        send_error(500, "Internal Error", (char *)0, "Config error - need rootdir specified.");
    rootdir = argv[1];
#endif
    params[0] = '\0';

    if (chdir(rootdir) < 0)
	    send_error(500, "Internal Error", (char*) 0, "Config error - couldn't chdir().");
    if (fgets(line, sizeof(line), stdin) == (char*)0)
	    send_error(400, "Bad Request", (char*) 0, "No request found.");
    if (sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3) 
	    send_error(400, "Bad Request", (char*) 0, "Can't parse request.");
    if ((result = resolve(path, ext, params)) == 1)
    {
        filepath[0] = '\0';
        strcat(filepath, rootdir);
        strcat(filepath, path);
        do_cgi(filepath, ext, params);
        return 0;
    }
    while (fgets(line, sizeof(line), stdin) != (char*) 0)
	    if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0)
	        break;
    if (strcasecmp(method, "get") != 0)
	    send_error(501, "Not Implemented", (char*) 0, "That method is not implemented.");
    if (path[0] != '/')
	    send_error(400, "Bad Request", (char*) 0, "Bad filename.");
    file = &(path[1]);
    strdecode(file, file);
    if (file[0] == '\0')
	    file = "./";
    len = strlen(file);
    if (file[0] == '/' || strcmp(file, "..") == 0 || strncmp(file, "../", 3) == 0 || strstr(file, "/../") != (char*) 0 || strcmp(&(file[len-3]), "/..") == 0)
	    send_error(400, "Bad Request", (char*) 0, "Illegal filename.");
    if (stat(file, &sb) < 0)
	    send_error(404, "Not Found", (char*) 0, "File not found.");
    if (S_ISDIR(sb.st_mode))
	{
	    if (file[len-1] != '/')
    	{
	        snprintf(location, sizeof(location), "Location: %s/", path);
	        send_error(302, "Found", location, "Directories must end with a slash.");
	    }
	    snprintf(idx, sizeof(idx), "%sindex.html", file);
	    if (stat(idx, &sb) >= 0)
    	{
	        file = idx;
	        goto do_file;
	    }
	    send_headers(200, "Ok", (char*) 0, "text/html", -1, sb.st_mtime);
	    printf("<html><head><title>Index of %s</title><style type='text/css'>a {text-decoration: none}</style></head>\n<body><h4>Index of %s</h4>\n<pre>\n", file, file);
	    n = scandir(file, &dl, NULL, alphasort);
	    if (n < 0)
	        perror("scandir");
	    else
	        for (i = 0; i < n; ++i)
		        file_details(file, dl[i]->d_name);
	    printf("</pre>\n<hr>\n<address><a href=\"%s\">%s</a></address>\n</body></html>\n", SERVER_URL, SERVER_NAME);
	}
    else
	{
	    do_file:
	    fp = fopen(file, "r");
	    if (fp == (FILE*) 0)
	        send_error(403, "Forbidden", (char*) 0, "File is protected.");
	    send_headers(200, "Ok", (char*) 0, get_mime_type(file), sb.st_size, sb.st_mtime);
	    while ((ich = getc(fp)) != EOF)
	        putchar(ich);
	}

    fflush(stdout);
    return 0;
}

static void file_details(char* dir, char* name)
{
    static char encoded_name[1000];
    static char path[2000];
    struct stat sb;
    char timestr[16];

    strencode(encoded_name, sizeof(encoded_name), name);
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    if (lstat(path, &sb) < 0)
	    printf("<a href=\"%s\">%-32.32s</a>    ???\n", encoded_name, name);
    else
	{
	    strftime(timestr, sizeof(timestr), "%d%b%Y %H:%M", localtime(&sb.st_mtime));
	    printf("<a href=\"%s\">%-32.32s</a>    %15s %14lld\n", encoded_name, name, timestr, (int64_t) sb.st_size);
	}
}


static void send_error(int status, char* title, char* extra_header, char* text)
{
    send_headers(status, title, extra_header, "text/html", -1, -1);
    printf("<html><head><title>%d %s</title></head>\n<body bgcolor=\"#cc9999\"><h4>%d %s</h4>\n", status, title, status, title);
    printf("%s\n", text);
    printf("<hr>\n<address><a href=\"%s\">%s</a></address>\n</body></html>\n", SERVER_URL, SERVER_NAME);
    fflush(stdout);
    exit(1);
}

static void send_headers(int status, char* title, char* extra_header, char* mime_type, off_t length, time_t mod)
{
    time_t now;
    char timebuf[100];

    printf("%s %d %s\015\012", PROTOCOL, status, title);
    printf("Server: %s\015\012", SERVER_NAME);
    now = time((time_t*) 0);
    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
    printf("Date: %s\015\012", timebuf);
    if (extra_header != (char*) 0)
	    printf("%s\015\012", extra_header);
    if (mime_type != (char*) 0)
	    printf("Content-Type: %s\015\012", mime_type);
    if (length >= 0)
	    printf("Content-Length: %lld\015\012", (int64_t) length);
    if (mod != (time_t) -1)
	{
	    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&mod));
	    printf("Last-Modified: %s\015\012", timebuf);
	}
    printf("Connection: close\015\012");
    printf("\015\012");
}

static char* get_mime_type(char* name)
{
    char* dot;

    dot = strrchr(name, '.');
    if (dot == (char*) 0)
	    return "text/plain; charset=iso-8859-1";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
	    return "text/html; charset=iso-8859-1";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
	    return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
	    return "image/gif";
    if (strcmp(dot, ".png") == 0)
	    return "image/png";
    if (strcmp(dot, ".css") == 0)
	    return "text/css";
    if (strcmp(dot, ".au") == 0)
	    return "audio/basic";
    if (strcmp(dot, ".wav") == 0)
	    return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
	    return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
	    return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
	    return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
	    return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
	    return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
	    return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
	    return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
	    return "application/x-ns-proxy-autoconfig";
    return "text/plain; charset=iso-8859-1";
}


static void strdecode(char* to, char* from)
{
    for (; *from != '\0'; ++to, ++from)
	{
	    if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
	    {
	        *to = hexit(from[1]) * 16 + hexit(from[2]);
	        from += 2;
	    }
	    else
	        *to = *from;
	}
    *to = '\0';
}


static int hexit(char c)
{
    if (c >= '0' && c <= '9')
    	return c - '0';
    if (c >= 'a' && c <= 'f')
	    return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
	    return c - 'A' + 10;
    return 0;	
}

static void strencode(char* to, size_t tosize, const char* from)
{
    int tolen;
    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from)
	{
    	if (isalnum(*from) || strchr("/_.-~", *from) != (char*) 0)
	    {
	        *to = *from;
	        ++to;
	        ++tolen;
	    }
	    else
	    {
	        sprintf(to, "%%%02x", (int) *from & 0xff);
	        to += 3;
	        tolen += 3;
	    }
	}
    *to = '\0';
}
