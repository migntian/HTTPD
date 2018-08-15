#include "get_line.h"

int get_line(int sock,char line[],int size)
{
    //一般我们的行分隔符是回车换行
    //但是实际上客户端发来的请求有可能是
    //\r
    //\n
    //\r\n
    //我们需要统一处理，统一按\n处理
    int c = 'a';
    int i = 0;
    ssize_t s = 0;
    while(i < size-1 && c != '\n')
    {
        s = recv(sock,&c,1,0);
        if(s > 0)
        {
            if(c == '\r')
            {
                recv(sock,&c,1,MSG_PEEK);
                if(c != '\n')
                {
                    c = '\n';
                }
                else
                {
                    recv(sock,&c,1,0);
                }
            }
            line[i++] = c;
        }
        else
        {
            break;
        }
    }
    line[i] = '\0';
    return i;
}
