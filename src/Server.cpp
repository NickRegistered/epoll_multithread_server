#include "Server.h"
std::mutex m_working;
bool working;

ThreadPool *pool;
int epfd;


map<int,string> clientAddrMap;

void writeLogLine(const char *str);
void replyTask(int clientFd);

Server::Server(const string root,const string IP,const unsigned short port):
    root(root),ipAddr(IP),port(port)
{  }

int Server::initServer()
{
    int ret,flag;
    srvFd = socket(AF_INET,SOCK_STREAM,0);
    if(srvFd == -1){
        printf("Create server socket failed!");
        return -1;
    }
    writeLogLine("Create server socket OK!");
    fcntl(srvFd,F_SETFL,O_NONBLOCK);


    /*set address reuse*/
    flag = 1;
    setsockopt(srvFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));

    /*set server's address & port*/
    struct sockaddr_in srvAddr;
    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    srvAddr.sin_port = htons(port);
    bind(srvFd,(sockaddr*)&srvAddr,sizeof(srvAddr));

    if((epfd = epoll_create1(EPOLL_CLOEXEC)) == -1){
        writeLogLine("epoll create failed!");
        return -1;
    }

    writeLogLine("Create epoll OK!");
    if((ret = listen(srvFd,MAX_EVENT))==-1){
        writeLogLine("Listen failed!");
        return -1;
    }
    writeLogLine("Server listen startup OK!");
    ev.data.fd = srvFd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,srvFd,&ev);
    writeLogLine("Server startup OK!");
    return 0;
}

int Server::acceptConnection(){
    struct sockaddr_in clientAddr;
    socklen_t nAddrLen;
    char logBuff[BUFFER_SIZE];
    nAddrLen = sizeof(clientAddr);
    int clientFd = accept(srvFd,(sockaddr*)&clientAddr,&nAddrLen);
    if(clientFd == -1){
        writeLogLine("accept failed!");
        return -1;
    }

    char clientIp[64] = {0};
    string IP(inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,clientIp,sizeof(clientIp)));
    clientAddrMap[clientFd] = IP;
    sprintf(logBuff,"One new client!\nIP : %s",IP.c_str());
    writeLogLine(logBuff);
    int ret = fcntl(clientFd,F_GETFL);
    ret |= O_NONBLOCK;
    fcntl(clientFd,F_SETFL,ret);

    ev.data.fd = clientFd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,clientFd,&ev);
    writeLogLine("One new session is established!");
    return 0;
}

void Server::run(){

    working = (initServer() != -1);
    pool = new ThreadPool(3);
    chdir(root.c_str());
    
    while(working){
        trigger_num = epoll_wait(epfd,events,MAX_EVENT,500);
        for(int i=0;i<trigger_num;++i){
            int curFd = events[i].data.fd;
            if(curFd == srvFd){//new connections
                acceptConnection();
            }
            else if((events[i].events&EPOLLIN)){//receive data from clinents
                pool->enqueue(replyTask,curFd);
            }
        }
    }
    return;
}

Server::~Server(){
    delete pool;
    epoll_ctl(epfd,EPOLL_CTL_DEL,srvFd,nullptr);
    close(epfd);
    writeLogLine("Server has been shut down.");
}

void replyTask(int clientFd){

     string IP;
     IP = clientAddrMap[clientFd];
     char recvBuff[BUFFER_SIZE];
     char sendBuff[BUFFER_SIZE];
     char logBuff[BUFFER_SIZE];
     memset(recvBuff,0,BUFFER_SIZE);
     ssize_t len = recv(clientFd,recvBuff,BUFFER_SIZE,0);
     if(len == 0){
         sprintf(logBuff,"One connection closed.\nIP : %s ",IP.c_str());
         writeLogLine(logBuff);
         epoll_ctl(epfd,EPOLL_CTL_DEL,clientFd,nullptr);
         return;
     }
     else if(len < 0) {
         sprintf(logBuff,"receive data from client failed! errno:%d\nIP : %s ",errno,IP.c_str());
         writeLogLine(logBuff);
         epoll_ctl(epfd,EPOLL_CTL_DEL,clientFd,nullptr);
         return;
     }
     Requestion req(recvBuff);

     sprintf(logBuff,"receive data from a client.\nIP : %s\
     					\nRequestion Method : %s\nRequestion URL : %s",
                                    IP.c_str(),
                                    req.Method,
                                    req.URL+1);
     writeLogLine(logBuff);

#ifdef DEBUG_ON
     cout<<"Method "<<req.Method<<endl;
     cout<<"URL "<<req.URL+1<<endl;
     cout<<"Proto "<<req.proto<<endl;
#endif


     if(strcmp(req.proto,"HTTP")){
        writeLogLine("resolve error!");
        return;
     }

     int fd;
     size_t ret;
     Response rsp(req.URL+1,req.Extn,sendBuff);
     if((ret = rsp.len)== (~0uL)){
         sprintf(logBuff,"open %s failed",req.URL+1);
         writeLogLine(logBuff);
         return ;
     }
     send(clientFd,sendBuff,ret,0);
     sprintf(logBuff,"sending file : %s",req.URL+1);
     writeLogLine(logBuff);
     fd = open(req.URL+1,O_RDONLY);
     while((ret = read(fd,sendBuff,BUFFER_SIZE))){
         send(clientFd,sendBuff,ret,0);
     }
     writeLogLine("file sending succeeded!");
     close(fd);
     return;

}

void writeLogLine(const char* str){
    time_t t;
    time(&t);
    char *time = ctime(&t);
    time[strlen(time)-1] = '\0';
	printf("\n[%s]\t%s\n",time,str);
    return;
}
