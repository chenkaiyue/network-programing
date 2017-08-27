https://www.cnblogs.com/skynet/archive/2010/12/12/1903949.html  


bzero() 会将内存块（字符串）的前n个字节清零，其原型为：
    void bzero(void *s, int n);
accept的第一个参数为服务器的socket描述字，是服务器开始调用socket()函数生成的，称为监听socket描述字；而accept函数返回的是已连接的socket描述字。一个服务器通常通常仅仅只创建一个监听socket描述字，它在该服务器的生命周期内一直存在。内核为每个由服务器进程接受的客户连接创建了一个已连接socket描述字，当服务器完成了对某个客户的服务，相应的已连接socket描述字就被关闭。

这里在子进程中，首先将listenfd关闭，因为fork函数后，子进程会获得父进程资源的副本，而子进程中并不需要listenfd，直接关闭免的出麻烦。在str_echo调用完成后，并没有使用close(connfd)，而是直接调用exit函数。这里我们看下exit函数究竟会做什么。


TCP server：  
创建listen_socket->server_addr初始化->bind(socket,server_addr)->listen(listen_socker) ->conn_socket=accept(listen_socket,client_addr)->read/write-close(conn_socket)  
client：  
创建套接字–初始化server的参数–connect连接Server–发送和接收字符串–退出程序（关闭套接字）  