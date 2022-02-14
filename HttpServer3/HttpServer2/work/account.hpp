#pragma once
#include "../tools/head.h"

void checkAccount(int clientfd,const std::string& url){
    using std::string;
    string::size_type pos=url.find("account=");
    if(pos==string::npos)return;
    string account=string(url.begin()+pos+8,url.end());
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    header.setStatusCode(200);
    Mysql_service service;
    if(service.checkAccount(account)>0)header.addJson("result:","success");
    else header.addJson("result:","fail");
    string json;
    header.setField("Content-Length",std::to_string(header.toJson(json)));
    string result=header.toString()+json;
    send(clientfd,result.c_str(),result.size(),0);
    close(clientfd);
}

void sendCode(int clientfd,const std::string& url){
    using std::string;
    string::size_type pos=url.find("account=");
    if(pos==string::npos)return;
    string account=string(url.begin()+pos+8,url.end());
    newrand nrand;
    Email email;
    int code=nrand.nextInt(100000,999999);
    string message="天速快传网提醒您，您的验证码为："+std::to_string(code)+"，请尽快完成验证。祝您生活愉快！";
    int ret=email.sendEmail(account,message);
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    header.setStatusCode(200);
    if(ret==0){//成功发送
        header.addJson("result","successSend");
        header.addJson("vericode",std::to_string(code));
    }
    else header.addJson("result:","failSend");
    string json;
    header.setField("Content-Length",std::to_string(header.toJson(json)));
    string result=header.toString()+json;
    send(clientfd,result.c_str(),result.size(),0);
    close(clientfd);
}

void login(int clientfd,const std::string& url){
    using std::string;
    string::size_type pos1=url.find("account=");
    string::size_type pos2=url.find("&password=");
    if(pos1==string::npos || pos2==string::npos)return;
    string account=string(url.begin()+pos1+8,url.begin()+pos2);
    string password=string(url.begin()+pos2+10,url.end());
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    header.setStatusCode(200);

    if(!Users::checkUser(account)){
        Mysql_service service;
        User user;
        if(service.login(account,password,user)>0){
            user.setFd(clientfd);
            Users::addUser(user);
            header.addJson("result","success");
            header.addJson("account",account);
            header.addJson("password",password);
        }
        else header.addJson("result","fail");
    }
    else{
        header.addJson("result","success");
        header.addJson("account",account);
        header.addJson("password",Users::getPassword(account));
    }
    
    string json;
    header.setField("Content-Length",std::to_string(header.toJson(json)));
    //header.setField("Access-Control-Allow-Origin","*");
    //header.setField("Access-Control-Allow-Method","POST,GET");
    string result=header.toString()+json;
    send(clientfd,result.c_str(),result.size(),0);
    close(clientfd);
}

void enroll(int clientfd,RequestHeader& request){
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    header.setStatusCode(200);
    Mysql_service service;
    if(service.enroll(request.getParameter("nickname"),
        request.getParameter("account"),request.getParameter("password"))>0){
            header.addJson("result","success");
        }
    else header.addJson("result","fail");
    std::string json;
    header.setField("Content-Length",std::to_string(header.toJson(json)));
    std::string result=header.toString()+json;
    send(clientfd,result.c_str(),result.size(),0);
    close(clientfd);
}

void resetPassword(int clientfd,const std::string& url){
    using std::string;
    string::size_type pos1=url.find("account=");
    string::size_type pos2=url.find("&password=");
    if(pos1==string::npos || pos2==string::npos)return;
    string account=string(url.begin()+pos1+8,url.begin()+pos2);
    string password=string(url.begin()+pos2+10,url.end());

    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    header.setStatusCode(200);
    Mysql_service service;
    if(service.resetPassword(account,password)>0){
        header.addJson("result","success");
        header.setField("Set-Cookie","account="+account+"; password="+password);
    }
    else header.addJson("result","fail");
    string json;
    header.setField("Content-Length",std::to_string(header.toJson(json)));
    string result=header.toString()+json;
    send(clientfd,result.c_str(),result.size(),0);
    close(clientfd);
}