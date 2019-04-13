# epoll_multithread_server
&emsp; **epoll+线程池实现WebServer**

&emsp;&emsp;为了节约线程资源。取消使用QT界面，直接通过命令行设置参数运行
&emsp;&emsp;通过设置软中断处理函数的方式，收到SIGINT信号(Ctrl+C)时结束程序运行

* 编译运行
![build&use](https://github.com/NickRegistered/epoll_multithread_server/blob/master/pic/build%26usage.png)

* 运行效果
![preview](https://github.com/NickRegistered/epoll_multithread_server/blob/master/pic/preview.png)

* 页面预览
![page](https://github.com/NickRegistered/epoll_multithread_server/blob/master/pic/page.png)

* 关闭服务器
![close](https://github.com/NickRegistered/epoll_multithread_server/blob/master/pic/close_server.png)