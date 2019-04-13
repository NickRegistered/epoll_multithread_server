#ifndef RESPONSE_H
#define RESPONSE_H

#include <sys/stat.h>
#include <string.h>
#include <map>
#include <iostream>
using namespace std;
class Response {
public:
	size_t len;
    char Stat[32];		//状态行
    char CntType[32];	//Content-Type;
    char CntLen[32];	//Content-Length
public:
    Response(const char *filename,char* Extn,char *sendBuff);//根据文件信息组织响应报文，写入buff中
};
#endif // RESPONSE_H
