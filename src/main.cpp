#include <signal.h>
#include "Server.h"

extern bool working;
Server* srv;

void sig_int_process(int sig_no){
    if(sig_no == SIGINT){
        working = false;
        delete srv;
    }
    return;
}


int main(int argc,const char *argv[])
{
    if(argc != 4){
        printf("Usage: ./Server <server_root_path> <IP> <port>\n");
        return -1;
    }

    signal(SIGINT,SIG_IGN);
    signal(SIGINT,sig_int_process);
    srv = new Server(argv[1],argv[2],atoi(argv[3]));
    working = true;
    srv->run();

    return 0;
}
