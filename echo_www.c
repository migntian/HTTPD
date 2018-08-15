#include <stdio.h>
#include "echo_www.h"
void echo_www(int sock,char *path,int size,int *err)
{
   clear_header(sock);
   //此时我们就需要响应，即把path里面数据读出来，写到sock里就完成响应了
   int fd = open(path,O_RDONLY);
   if(fd < 0)
   {
       *err = 404;
       return;
   }
   char line[MAX];
   sprintf(line,"HTTP/1.0 200 OK\r\n");
   send(sock,line,strlen(line),0);
   sprintf(line,"Content-Type:text/html\r\n");
   send(sock,line,strlen(line),0);
   sprintf(line,"\r\n");
   send(sock,line,strlen(line),0);
   sendfile(sock,fd,NULL,size);
   close(fd);
}
