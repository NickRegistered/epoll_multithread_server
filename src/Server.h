#ifndef SERVER_H
#define SERVER_H

#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <map>
#include <string>
#include "ThreadPool.h"
#include "Requestion.h"
#include "Response.h"

using namespace std;

const int MAX_EVENT = 1024;
const int BUFFER_SIZE = 1024;


class Server
{
public:

    Server();
    Server(const string root,const string IP,const unsigned short port);
    ~Server();
    void run();
private:
    const string root;
    const string ipAddr;
    const unsigned short port;

    int srvFd;
    struct epoll_event ev,events[MAX_EVENT];

    int trigger_num;
private:
    int initServer();
    int acceptConnection();
};

#endif // SERVER_H
