#ifndef MUDUO_NET_HTTP_MIDTREAT_H
#define MUDUO_NET_HTTP_MIDTREAT_H

#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpResponse.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"

#include "SqlService.hpp"
#include "tools/newrand.hpp"
#include "tools/email.hpp"
#include "tools/encrypt.hpp"

#include <sstream>
#include <fstream>
#include <sys/stat.h>
using namespace muduo;
using namespace muduo::net;

namespace muduo{
namespace net{

    class Midtreat : noncopyable{
    public:
        Midtreat()=default; 
        void userClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void fileClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void otherClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

    private:
        void checkNickname(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void checkAccount(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void enroll(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void login(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
        void resetPassword(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

    private:
        void upload(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
    private:
        void responseSuccess(const TcpConnectionPtr& conn,HttpResponse& res);
        void sendEmailCode(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
    };


    }  // namespace net
}  // namespace muduo

void Midtreat::responseSuccess(const TcpConnectionPtr& conn,HttpResponse& res){
    res.setStatusCode(HttpResponse::k200Ok);
    res.setStatusMessage("OK");
    res.setCloseConnection(true);
    Buffer buf;
    res.appendToBuffer(&buf);
    conn->send(&buf);
    if (res.closeConnection())conn->shutdown();
}

void Midtreat::sendEmailCode(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string account=buffer->retrieveAllAsString();
    newrand nrand;
    Email email;
    int code=nrand.nextInt(100000,999999);
    string message="天速快传网提醒您，您的验证码为："+std::to_string(code)+"，请尽快完成验证。祝您生活愉快！";
    int ret=email.sendEmail(account,message);
    if(ret==0){//成功发送
        LOG_INFO << "验证码发送成功!";
        std::string result="{\"result\":\"sendEmailSuccess\",\"vericode\":\""+std::to_string(code)+"\"}";
        res.setBody(result);
        LOG_INFO << "account="<<account<<";vericode="<<code;
    }
    else {
        LOG_INFO << "验证码发送失败!";
        std::string result2="{\"result\":\"sendEmailfail\"}";
        res.setBody(result2);
    }
    responseSuccess(conn,res);
}

void Midtreat::userClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    if(req.path().find("checkNickname")!=string::npos)checkNickname(conn,req,res);
    else if(req.path().find("checkAccount")!=string::npos)checkAccount(conn,req,res);
    else if(req.path().find("enroll")!=string::npos)enroll(conn,req,res);
    else if(req.path().find("login")!=string::npos)login(conn,req,res);
    else if(req.path().find("resetPassword")!=string::npos)resetPassword(conn,req,res);
 }

void Midtreat::fileClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    if(req.path().find("upload")!=string::npos)upload(conn,req,res);
}

 void Midtreat::otherClassify(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    if(req.path().find("sendEmailCode")!=string::npos)sendEmailCode(conn,req,res);
 }

void Midtreat::checkNickname(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string nickname=buffer->retrieveAllAsString();
    std::string text="SELECT nickname FROM user where nickname='" + nickname + "' LIMIT 10;";
    signaling::Signaling signal;
    signal.set_command("selectUser");
    signal.set_text(text);
    sql::SqlService service;
    std::string result=service.sendSql(signal);
    if(signal.ParseFromString(result)){
        LOG_INFO << "返回消息解析成功!";
        if(signal.users().size()==0 || signal.users()[0].nickname()=="")res.setBody("not-exists:nickname");
        else res.setBody("exists:nickname");
    }
    else {
        LOG_INFO << "返回消息解析失败!";
    }
    responseSuccess(conn,res);
}

void Midtreat::checkAccount(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string account=buffer->retrieveAllAsString();
    std::string text="SELECT account FROM user where account='" + account + "' LIMIT 10;";
    signaling::Signaling signal;
    signal.set_command("selectUser");
    signal.set_text(text);
    sql::SqlService service;
    std::string result=service.sendSql(signal);
    if(signal.ParseFromString(result)){
        LOG_INFO << "返回消息解析成功!";
        if(signal.users().size()==0 || signal.users()[0].account()=="")res.setBody("not-exists:account");
        else res.setBody("exists:account");
    }
    else {
        LOG_INFO << "返回消息解析失败!";
    }
    responseSuccess(conn,res);
}

void Midtreat::enroll(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string mess=buffer->retrieveAllAsString();
    std::stringstream ss(mess);
    std::string word;
    transfer::User user;
    while(getline(ss,word,'&')){
        string::size_type pos=word.find_first_of("=");
        if(pos!=string::npos){
            std::string key(word.begin(),word.begin()+pos);
            std::string value(word.begin()+pos+1,word.end());
            if(key=="nickname")user.set_nickname(value);
            else if(key=="account")user.set_account(value);
            else if(key=="password")user.set_password(value);
        }
    }
    signaling::Signaling signal;
    signal.set_command("insertUser");
    transfer::User* userp=signal.add_users();
    *userp=user;
    sql::SqlService service;
    std::string result=service.sendSql(signal);
    if(signal.ParseFromString(result)){
        LOG_INFO << "返回消息解析成功!";
        res.setBody(signal.command()+":"+signal.text());
    }
    else {
        LOG_INFO << "返回消息解析失败!";
    }
    responseSuccess(conn,res);
}


void Midtreat::login(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string mess=buffer->retrieveAllAsString();
    std::string::size_type pos1=mess.find("account=");
    std::string::size_type pos2=mess.find("&password=");
    if(pos1==string::npos || pos2==string::npos)return;
    std::string account=string(mess.begin()+pos1+8,mess.begin()+pos2);
    std::string password=string(mess.begin()+pos2+10,mess.end());
    std::string text="SELECT account FROM user where account='";
    text+=account+"' AND password='"+password+"' LIMIT 10;";
    signaling::Signaling signal;
    signal.set_command("selectUser");
    signal.set_text(text);
    sql::SqlService service;
    std::string result=service.sendSql(signal);
    if(signal.ParseFromString(result)){
        LOG_INFO << "返回消息解析成功!";
        if(signal.users().size()==0 || signal.users()[0].account()=="")res.setBody("login:fail");
        else res.setBody("login:success"); 
    }
    else {
        LOG_INFO << "返回消息解析失败!";
    }
    responseSuccess(conn,res);
}

void Midtreat::resetPassword(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    Buffer* buffer=conn->inputBuffer();
    std::string mess=buffer->retrieveAllAsString();
    std::string::size_type pos1=mess.find("account=");
    std::string::size_type pos2=mess.find("&password=");
    if(pos1==string::npos || pos2==string::npos)return;
    std::string account=string(mess.begin()+pos1+8,mess.begin()+pos2);
    std::string password=string(mess.begin()+pos2+10,mess.end());
    std::string text="UPDATE user SET password='"+password+"' WHERE account='"+account+"';";
    signaling::Signaling signal;
    signal.set_command("updateUser");
    signal.set_text(text);
    sql::SqlService service;
    std::string result=service.sendSql(signal);
    if(signal.ParseFromString(result)){
        LOG_INFO << "返回消息解析成功!";
        res.setBody(signal.command()+":"+signal.text());
    }
    else {
        LOG_INFO << "返回消息解析失败!";
    }
    responseSuccess(conn,res);
}

void Midtreat::upload(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
    std:;string url=codec::decodeURL(req.path());
    std::string::size_type pos1=url.find("fileSize=");
    std::string::size_type pos2=url.find("&fileName=");
    long long fileSize=atol(std::string(url.begin()+pos1+9,url.begin()+pos2).c_str());
    std::string fileName=std::string(url.begin()+pos2+10,url.end());
    std::cout<<"fileSize:"<<fileSize<<"fileName:"<<fileName<<std::endl;

    newrand nrand;
    std::string fileCode=nrand.nextDigitAndLetter(20);
    std::string WEB_PATH="/home/HttpServer2/web/download/";
    std::string filePath=WEB_PATH+fileCode+"/"+fileName;

    signaling::Signaling signal;
    signal.set_command("insertFile");
    transfer::File file;
    file.set_filesize(fileSize);
    file.set_filename(fileName);
    file.set_filecode(fileCode);
    file.set_filepath(filePath);
    transfer::File* filep=signal.add_files();
    *filep=file;
    sql::SqlService service;
    while(true){
        std::string temp=service.sendSql(signal);
        bool flag=signal.ParseFromString(temp);
        if(flag){
           if(signal.text()=="success")break;
           else{
            fileCode=nrand.nextDigitAndLetter(20);
            filePath=WEB_PATH+fileCode+"/"+fileName;
            signal.clear_files();
            file.set_filecode(fileCode);
            file.set_filepath(filePath);
            *signal.add_files()=file;
           }
        }
        else {
            LOG_INFO << "返回消息解析失败!";
            break;
        }
    }
    
    string dirpath=WEB_PATH+fileCode+"/";
    if(mkdir(dirpath.c_str(),S_IRWXU)<0){
       LOG_INFO << "目录创建失败!";
        //service.deleteFile(fileCode);
        //break;
    }
    else{
        /*std::ofstream outfile(filePath, std::ios::out | std::ios::binary);
        if(outfile.is_open()){
            const int BUFFER_SIZE=1024*1024;
            char buff[BUFFER_SIZE];
            memset(buff,0,BUFFER_SIZE);
            int readSize=0,readBytes=0;
            int fd=conn->fd();
            while(fileSize>0){
                readSize=fileSize<BUFFER_SIZE?fileSize:BUFFER_SIZE;
                do{
                    readSize-=recv(fd,buff,readSize,0);
                    outfile<<buff;
                    memset(buff,0,BUFFER_SIZE);
                }while(readSize>0);
                fileSize-=readSize;
            }
        }
        else LOG_INFO << "打开文件失败!";
        outfile.close();*/
    }
    
    
    /*const int BUFFER_SIZE=1024;
    int spsize=BUFFER_SIZE;
    while(fileSize>0){
        if(fileSize<BUFFER_SIZE)spsize=fileSize;
        outfile<<buffer->retrieveAsString(spsize);
        fileSize-=spsize;
        spsize=BUFFER_SIZE;
    }*/
    
    
}

#endif  // MUDUO_NET_HTTP_MIDTREAT_H
