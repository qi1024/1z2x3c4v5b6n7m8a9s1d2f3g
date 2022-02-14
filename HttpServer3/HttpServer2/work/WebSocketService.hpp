#pragma once
#include <iostream>
#include "../tools/head.h"

void wconnect(int clientfd,RequestHeader& header,int epollfd){
    using std::string;
    string key;
    header.getFields("Sec-WebSocket-Key",key);
    key+="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    char value[1024]={0};
    SHA1_encrypt(key.c_str(),key.size(),value);

    char temp[1024]={0};
    for(int i=0,size=strlen(value);i<size;i+=2)temp[i/2]=htoi(value,i,2);    
    key=base64_encode(temp,strlen(temp));

    ResponseHeader response;
    response.setVersion("HTTP/1.1");
    response.setStatusCode(101);
    response.setField("Connection","Upgrade");
    response.setField("Upgrade","websocket");
    response.setField("Sec-WebSocket-Accept",key);

    string result=response.toString();
    send(clientfd,result.c_str(),result.size(),0);


    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd=clientfd;
    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&event)<0){
        LOG::getInstance()->log(__FILE__,__func__,__LINE__,string("epoll添加wsfd失败")+strerror(errno));
    }
    else{
        LOG::getInstance()->log(__FILE__,__func__,__LINE__,"epoll添加wsfd成功");
    }     
}


void onlineRequest(int clientfd,WsRequestHeader& header){
    std::string srcaccount=header.getParameter("srcaccount");
    std::string targetAccount=header.getParameter("targetAccount");
    std::string password=header.getParameter("password");
    //Users::checkUser(srcaccount,password);
    WsResponseHeader response;
    if(!Users::checkUser(srcaccount,password) || !Users::checkUser(targetAccount)){
        response.addParameter("command","onlineRequestResult");
        response.addParameter("result","offline");
        std::string message=packData(response.toJson());
        send(clientfd,message.c_str(),message.size(),0);
    }
    else{
        std::string nickname=Users::getNickname(srcaccount);
        response.addParameter("command","onlineRequest");
        response.addParameter("account",srcaccount);
        response.addParameter("nickname",nickname);
        response.addParameter("fileName",header.getParameter("fileName"));
        response.addParameter("fileSize",header.getParameter("fileSize"));
        std::string message=packData(response.toJson());
        //std::cout<<"在线传输请求信息："+message<<std::endl;
        send(Users::getFd(targetAccount),message.c_str(),message.size(),0);
    }
}

void onlineResponse(int clientfd,WsRequestHeader& header){
    std::string account=header.getParameter("account");
    std::string targetAccount=header.getParameter("targetAccount");
    if(Users::checkUser(targetAccount) &&Users::checkUser(account)){
        WsResponseHeader response;
        response.addParameter("command","onlineResponse");
        response.addParameter("account",account);
        response.addParameter("result",header.getParameter("result"));
        response.addParameter("nickname",Users::getNickname(account));
        response.addParameter("fileKey",header.getParameter("fileKey"));
        std::string result=packData(response.toJson());
        //std::cout<<"在线传输应答信息："+result<<std::endl;
        send(Users::getFd(targetAccount),result.c_str(),result.size(),0);
    }
    else std::cout<<"对方不在线"<<std::endl;
}



    

    




