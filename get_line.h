#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include "listensock.h"

#define MAX 1024
#define HOME_PAGE "index.html"

int get_line(int sock,char line[],int size);
