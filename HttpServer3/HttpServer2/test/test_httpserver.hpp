#pragma once
#include <stdlib.h>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include "../tools/epoll_tool.hpp"


class test_httpserver{

public:
test_httpserver(){}
~test_httpserver(){}


public:

    void run(){
        request.append("GET /test HTTP/1.1\r\n");
        request.append("Connection: close\r\n");
        request.append("User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.77 Safari/537.36\r\n");
        request.append("Accept-Language: zh-CN,zh;q=0.9\r\n");
        request.append("Accept-Encoding: gzip, deflate, br\r\n");
        request.append("Upgrade-Insecure-Requests: 1\r\n");
        request.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n");
        request.append("Connection: keep-alive\r\n");
        request.append("Sec-Fetch-Dest: document\r\n");
        request.append("Host: 127.0.0.1:9090\r\n\r\n");
        /*std::cout<<"request:"<<std::endl;
        std::cout<<request<<std::flush;*/

        std::ios::sync_with_stdio(false);
        struct sockaddr_in serv_addr;
        int socketfd = -1;
        int ret=-1;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_addr.s_addr=inet_addr(server_ip.c_str());
        serv_addr.sin_port = htons(server_port);
        serv_addr.sin_family = AF_INET;
        for(int i=1;i<=100000;++i){
            socketfd=socket(AF_INET, SOCK_STREAM, 0);
            ret=connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
            ret=send(socketfd,request.c_str(),request.size(),0);
            std::cout<<"第"<<i<<"个连接建立完成"<<std::endl;
            close(socketfd);
        }
    }

private:
    char read_buffer[1024];
    const std::string server_ip="127.0.0.1";
    const int server_port=9090;
    std::string request;
    
};