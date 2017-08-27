//tcpserver
#include    "unp.h"

int
main(int argc, char **argv)
{
    int  listenfd, connfd;//监听socket，已连接socket
    pid_t    childpid;
    socklen_t    clilen;
    struct sockaddr_in  cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

    if ( (childpid = Fork()) == 0) {    /* child process */
    Close(listenfd);    /* close listening socket */
    str_echo(connfd);   /* process the request */
    exit(0);
    }
    Close(connfd);   /* parent closes connected socket */
    }
    /*这里在子进程中，首先将listenfd关闭，因为fork函数后，子进程会获得父进程资源的副本，而子进程中并不需要listenfd，直接关闭免的出麻烦。在str_echo调用完成后，并没有使用close(connfd)，而是直接调用exit函数,exit调用之后，会调用退出处理函数，这里就是一些对资源的释放，包括已打开的套接字。
    */
}


void
str_echo(int sockfd)
{
    ssize_t  n;
    char     buf[MAXLINE];

    again:
    while ( (n = read(sockfd, buf, MAXLINE)) > 0)
    Writen(sockfd, buf, n);

    if (n < 0 && errno == EINTR)
    goto again;
    else if (n < 0)
    err_sys("str_echo: read error");
}