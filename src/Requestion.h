#ifndef REQUESTION_H
#define REQUESTION_H
#include <string.h>

#ifdef DEBUG_ON
#include<iostream>
using namespace std;
#endif

class Requestion{
public:
    char Method[8]; //请求方法
    char Extn[8];   //请求文件扩展名
    char proto[8];
    char URL[64];  //请求文件路径
public:
     Requestion(char *req);

};
#endif // REQUESTION_H
