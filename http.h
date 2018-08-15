#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include "listensock.h"
#include "get_line.h"
#include "echo_www.h"

#define MAX 1024
#define HOME_PAGE "index.html"

typedef struct SockArr{
    int web_sock;
    int client_sock;
}SockA;

int get_line(int sock,char line[],int size);
void clear_header(int sock);
void echo_error(int code);
int exe_cgi(SockA *s,char path[],char method[],char *query_string);//核心
void *handler_request_web(void *arg);
