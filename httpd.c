#include "http.h"
// #include "get_line.h"

//typedef struct SockArr{
//    int web_sock;
//    int client_sock;
//}SockA;

//int get_line(int sock,char line[],int size)
//{
//    //一般我们的行分隔符是回车换行
//    //但是实际上客户端发来的请求有可能是
//    //\r
//    //\n
//    //\r\n
//    //我们需要统一处理，统一按\n处理
//    int c = 'a';
//    int i = 0;
//    ssize_t s = 0;
//    while(i < size-1 && c != '\n')
//    {
//        s = recv(sock,&c,1,0);
//        if(s > 0)
//        {
//            if(c == '\r')
//            {
//                recv(sock,&c,1,MSG_PEEK);
//                if(c != '\n')
//                {
//                    c = '\n';
//                }
//                else
//                {
//                    recv(sock,&c,1,0);
//                }
//            }
//            line[i++] = c;
//        }
//        else
//        {
//            break;
//        }
//    }
//    line[i] = '\0';
//    return i;
//}

//void echo_www(int sock,char *path,int size,int *err)
//{
//   clear_header(sock);
//   //此时我们就需要响应，即把path里面数据读出来，写到sock里就完成响应了
//   int fd = open(path,O_RDONLY);
//   if(fd < 0)
//   {
//       *err = 404;
//       return;
//   }
//   char line[MAX];
//   sprintf(line,"HTTP/1.0 200 OK\r\n");
//   send(sock,line,strlen(line),0);
//   sprintf(line,"Content-Type:text/html\r\n");
//   send(sock,line,strlen(line),0);
//   sprintf(line,"\r\n");
//   send(sock,line,strlen(line),0);
//   sendfile(sock,fd,NULL,size);
//   close(fd);
//}

void clear_header(int sock)
{
    char line[MAX];
    do{
        get_line(sock,line,sizeof(line));
    }while(strcmp(line,"\n") != 0);
}

void echo_error(int code)
{
    switch(code)
    {
    case 404:
        break;
    case 501:
        break;
    default:
        break;
    }
}

int exe_cgi(SockA *s,char path[],char method[],char *query_string)
{
    int sock = s->web_sock;
    int client_sock = s->client_sock;

    char line[MAX];
    int content_lenth = 0;

    char method_env[MAX/32];
    char query_string_env[MAX];
    char content_lenth_env[MAX/16];
    if(strcasecmp(method,"GET") == 0) 
    {
        clear_header(sock);
    }
    else
    {
        //POST
        do{
            get_line(sock,line,sizeof(line));
            if(strncmp(line,"content_lenth: ",16) == 0)
            {
                content_lenth = atoi(line+16);
            }
        }while(strcmp(line,"\n") != 0);
        if(content_lenth == 0)
        {
            return 404;
        }
    }

    sprintf(line,"HTTP/1.0 200 OK\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"Content-Type:text/html\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);

    int input[2];
    int output[2];

    pipe(input);
    pipe(output);

    pid_t id = fork();
    if(id < 0)
    {
        return 404;
    }
    else if(id == 0)
    {
        close(input[1]);
        close(output[0]);
        dup2(input[0],0);
        dup2(output[1],1);

        sprintf(method_env,"METHOD_ENV=%s",method);
        putenv(method_env);
        if(strcasecmp(method,"GET") == 0)
        {
            printf("%s\n",method);
            sprintf(query_string_env,"QUERY_STRING=%s",query_string);
            putenv(query_string_env);
        }
        else
        {
            sprintf(content_lenth_env,"CONTENT_LENGTH=%d",content_lenth);
            putenv(content_lenth_env);
        }
        execl(path,path,NULL);
        sleep(1);
        exit(1);
    }
    else
    {
        close(input[0]);
        close(output[1]);
        char c;
        if(strcasecmp(method,"POST") == 0)
        {
            int i = 0;
            while(i < content_lenth)
            {
                read(sock,&c,1);
                write(input[1],&c,1);
            }
        }
        while(read(output[0],&c,1) > 0)
        {
            send(sock,&c,1,0);
        }

        waitpid(id,NULL,0);
        close(input[1]);
        close(output[0]);
    char buf[] = "please goto !\n";
    write(client_sock, buf, sizeof(buf));
    }
    return 200;
}
//void *handler_request_client(void *arg)
//{
//    int new_sock_client = (int)arg;
//    char buf[1024] = {0};
//    fgets(buf,sizeof(buf),stdin);
//    buf[sizeof(buf)] = '\0';
//    write(new_sock_client,buf,strlen(buf));
//    while(1)
//    {
//        printf("server# ");
//     //   fgets(buf,sizeof(buf),stdin);
//     //   buf[sizeof(buf)] = '\0';
//     //   write(new_sock_client,buf,strlen(buf));
//        printf("please wait,rescu is going...\n");
//        break;
//    }
//    return;
//}

void *handler_request_web(void *arg)
{
    SockA* s = (SockA*)arg;
    int sock = s->web_sock;

    char line[MAX];
    char method[MAX/32];
    char url[MAX];
    char path[MAX];
    int errCode = 200;
    int cgi = 0;
    char *query_string = NULL;

    int ret = 0;
    if(ret = get_line(sock,line,sizeof(line)) < 0)
    {
        errCode = 404;
        goto end;
    }
    
    //get method
    //line[] = get / HTTP/1.1
    int i = 0;
    int j = 0;
    while(i < sizeof(method)-1 && j < sizeof(line) && !isspace(line[j]))
    {
        //i j  不能越界
        method[i] = line[j];
        i++,j++;
    }
    method[i] = '\0';

    if(strcasecmp(method,"GET") == 0){

    } 
    else if(strcasecmp(method,"POST"))
    {
        cgi = 1;
    }
    else
    {
        errCode = 404;
        goto end;
    }
    while(j < sizeof(line) && isspace(line[j]))
    {
        j++;
    }

    i = 0;

    while(i < sizeof(url)-1 && j < sizeof(line) && !isspace(line[j]))
    {
        url[i] = line[j];
        i++,j++;
    }
    url[i] = '\0';
    
    //url
    if(strcasecmp(method,"GET") == 0)
    {
        query_string = url;
        while(*query_string)
        {
            if(*query_string == '?')
            {
                *query_string = '\0';
                query_string++;
                cgi = 1;
                break;
            }
            query_string++;
        }
    }
    //method[GET,POST],cgi[0|1],url[](url 里面就是请求资源) ,query_string[NULL|arg]
    sprintf(path,"wwwroot%s",url);
    if(path[strlen(path)-1] == '/')
    {
        strcat(path,HOME_PAGE);
    }
    struct stat st;
    //是否有效

    printf("path:%s\n",path);
    printf("methods:%s\n",method);
    
    if(stat(path,&st) < 0)
    {
        errCode = 404;
        goto end;
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,HOME_PAGE);
        }
        else
        {
            if((st.st_mode & S_IXUSR) ||(st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
            {
                cgi = 1;
            }
        }
        if(cgi)
        {
            errCode = exe_cgi(s,path,method,query_string);
        }
        else
        {
            //cgi == 0 且是get方法，普通文件
            //method[GET,POST],cgi[0|1],url[](url 里面就是请求资源) ,query_string[NULL|arg]
            printf("path  %s\n", path);
            echo_www(sock,path,st.st_size,&errCode);
        }
    }



//#endif
end:
    if(errCode != 200)
    {
        echo_error(errCode);
    }
    close(sock);
}

int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("usage %s [PORT1] [PORT2]\n",argv[0]);
        return 1;
    }
    int sock1 = httpbind(argv[1]);
    int sock2 = httpbind(argv[2]);
    signal(SIGPIPE, SIG_IGN);

    struct sockaddr_in client;
    socklen_t len2 = sizeof(client);
    int new_sock_client = accept(sock2,(struct sockaddr*)&client,&len2);
    printf("%d\n",new_sock_client);

    for(;;)
    {
        struct sockaddr_in client_web;
        socklen_t len1 = sizeof(client_web);
        int new_sock_web = accept(sock1,(struct sockaddr*)&client_web,&len1);
        printf("%d\n",new_sock_web);

        SockA* s = (SockA*)malloc(sizeof(SockA));

        s->web_sock = new_sock_web;
        s->client_sock = new_sock_client;

        if(new_sock_web < 0)
        {
            printf("accept error\n");
            continue;
        }
        if(new_sock_client < 0)
        {
            printf("accept error\n");
            continue;
        }
        pthread_t id_web = 0;
        pthread_create(&id_web,NULL,handler_request_web,(void *)s);
        pthread_detach(id_web);
    }
}
