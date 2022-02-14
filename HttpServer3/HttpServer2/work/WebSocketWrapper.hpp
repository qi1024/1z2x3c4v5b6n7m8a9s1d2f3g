#pragma once
#include "websocket.hpp"

class WebSocketWrapper{
public:
    WebSocketWrapper(int cfd):clientfd(cfd){}
    WebSocketWrapper(int cfd,int epoll_fd):clientfd(cfd),epoll_fd(epoll_fd){}

    void process(){WebSocket::wsprocess(clientfd);}
    ~WebSocketWrapper()=default;
private:
    int clientfd=-1;
    int epoll_fd=-1;
};