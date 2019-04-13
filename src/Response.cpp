#include "Response.h"
using namespace std;
const map<string, string> ExtnToType = { { ".gif","image/gif" },{ ".mp3","audio/mp3" } ,
                                        { ".ogg","application/ogg" },{ ".mp4","audio/mp4" } ,
                                        { ".webm","video/webm" },{ ".html","text/html" },
                                         {".ico","application/x-ico"},{ "","text/html" } };

const static char* page404 = "<html><body><h1 style=\"text-align:center\">404 NotFound</h1></body></html>";
Response::Response(const char *filename,char* Extn,char *sendBuff){
    sendBuff[0] = '\0';
    struct stat st;
    int ret = stat(filename,&st);

    if (ret == -1) {
        Stat[0] = '\0';
        strcat(Stat, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 73\r\n\r\n");//文件打开失败，写入状态信息404
        strcat(sendBuff, Stat);
        strcat(sendBuff,page404);//直接写入404页面
        len = (~0uL);//-1 in unsigned long
    }
    else {
        Stat[0] = '\0';
        strcat(Stat,"HTTP/1.1 200 OK\r\n");

        auto itor = ExtnToType.find(Extn);
        sprintf(CntType,"Content-Type: %s\r\n",(itor->second).c_str());
        sprintf(CntLen,"Content-Length: %ld\r\n\r\n",st.st_size);

        strcat(sendBuff, Stat);
        strcat(sendBuff, CntType);
        strcat(sendBuff, CntLen);
#ifdef DEBUG_ON
    cout<<"sendBuff :"<<sendBuff;
#endif
        len = strlen(sendBuff);
    }
}