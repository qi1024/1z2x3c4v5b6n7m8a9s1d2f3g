#pragma once
#include "http_work.hpp"
class httpwork_wrapper{
public:
    httpwork_wrapper(int epofd,int cfd):epollfd(epofd),clientfd(cfd){}
    void process(){http_work::process(epollfd,clientfd);}
    ~httpwork_wrapper()=default;
private:
    int epollfd=-1;
    int clientfd=-1;
};