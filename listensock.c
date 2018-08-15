#include "listensock.h"
int httpbind(char *port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("socket error\n");
        return 2;
    }

    int opt = 1;//服务器主动断开的时候，要保证服务器不能因为TIME_WAIT问题而不能立即重启
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(port));
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
        printf("bind errno\n");
        return 3;
    }
    if(listen(sock,5) < 0)
    {
        printf("listen errno\n");
        return 4;
    }
    return sock;
}





























