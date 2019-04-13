#include "Requestion.h"
#include <iostream>

using namespace std;
Requestion::Requestion(char *req){
    int i=0;
    int len = strlen(req);
    Method[0] = '\0';Extn[0]='\0';URL[0]='\0';proto[0] = '\0';

    for(;req[i] != ' '&&i<len;++i) Method[i] = req[i];
    Method[i++] = '\0';//解析出Method字段，添加结束符

    int j=0,k=0,flag=0;
    for(;req[i] != ' '&&i<len;++i,++j){
        URL[j] = req[i];

        Extn[k] = req[i];
        if(req[i] == '.')flag = 1;//当检测到扩展名前的'.'时开始记录扩展名
        k+=flag;
    }URL[j] = '\0';Extn[k] = '\0';

    for(++i,j=0;req[i] != '/'&& i<len;++i,++j){
        proto[j] = req[i];
    }proto[j] = '\0';

    if(strcmp(URL,"/") == 0 || strlen(URL)==0){//当请求只有根目录时，默认发送index.html
        strcat(URL,"index.html");
    }

#ifdef DEBUG_ON
    cout<<req<<endl;
#endif

}
