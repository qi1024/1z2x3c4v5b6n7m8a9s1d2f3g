#pragma once
#include "../tools/head.h"
#include "transfer.hpp"
#include "account.hpp"
#include "websocket.hpp"

class http_work{
public:
    http_work()=default;
    ~http_work(){}

private:

    static void parseRequestHeader(RequestHeader& header,int clientfd){
        bool isFirstLine=true;
        bool isFinish=false;
        std::stringstream ssline;
        std::string line,key,value;
        //MSG_DONTWAIT
        char c;
        while(read(clientfd,&c,1)){
            if(c!='\r')line.push_back(c);
            else if(read(clientfd,&c,1) && c=='\n'){
                if(isFirstLine){//请求行
                    ssline<<line;
                    std::string method,url,version;
                    if(ssline>>method)header.setMethod(method);
                    if(ssline>>url)header.setUrl(url);
                    if(ssline>>version)header.setVersion(version);
                    isFirstLine=false;
                }
                else if(ssline<<line && getline(ssline,key,':')&& getline(ssline,value)){
                    header.addFields(key,value.erase(0,1));
                }
                ssline.clear(); 
                line.clear();
                if(read(clientfd,&c,1) && c=='\r'){
                    read(clientfd,&c,1);
                    break;
                }
                else line.push_back(c);
            }  
        }


        /*char c;
        std::stringstream ss;
        std::string buffer;
        while(read(clientfd,&c,1)>0){
           if(c!='\n')buffer.push_back(c);
            else{
                read(clientfd,&c,1);
                if(c=='\r'){read(clientfd,&c,1);break;}
            }
            buffer.push_back(c);
        }
        std::cout<<"buffer:"<<buffer<<std::flush;

        std::string method,url,version;
        ss<<buffer;
        ss>>method>>url>>version;

*/

    }

    static void returnIndexPage(int clientfd,const std::string& url,const std::string& cookie){
        using std::string;
        const std::string WEB_PATH="/home/huawei/project/HttpServer2/web";
        string file_path=WEB_PATH+url;
        struct stat file_info;
        stat(file_path.c_str(), &file_info);
        size_t fileSize = file_info.st_size;
        int filefd = open(file_path.c_str(),O_RDONLY);
        if(filefd<0){
            LOG::getInstance()->log(__FILE__,__func__,__LINE__,"访问路径非法"+file_path);
            close(filefd);
            return ;
        }

        std::string contentType="text/html";
        if(url.find(".css")!=string::npos){contentType="text/css";}
        else if(url.find(".js")!=string::npos)contentType="application/x-javascript";
        else if(url.find(".png")!=string::npos)contentType="image/png";
        else if(url.find(".ico")!=string::npos)contentType="image/x-icon";
        ResponseHeader header("HTTP/1.1",200);
        header.setField("Content-Length",std::to_string(fileSize));
        header.setField("Content-Type",contentType+"; charset=UTF-8");
        
        if(cookie!="")header.setField("set-Cookie",cookie);
        std::string result=header.toString();
        send(clientfd,result.c_str(),result.size(),0);
        //std::cout<<"clientfd:"<<clientfd<<"发送完成"<<std::endl;
        int send_bytes=sendfile(clientfd,filefd,nullptr,fileSize);
        if(send_bytes<0)LOG::getInstance()->log(__FILE__,__func__,__LINE__,"errno:"+std::to_string(errno)+":"+strerror(errno));
        close(filefd);
        close(clientfd);
    }


    static void parseRequestBody(RequestHeader& header,int clientfd){
        char temp[128]={0};
        if(recv(clientfd,temp,128,0)>0){ header.parseParam(temp);}
    }

public:

public:
    static void process(int epollfd,int clientfd){
        using std::string;
        RequestHeader header;
        parseRequestHeader(header,clientfd);
        string url=header.getUrl();
        string cookie="";
        header.getFields("Cookie",cookie);
        if(url=="/" || url=="/index" || url=="/index.html"){ url="/index.html"; header.setUrl(url);}
        if(url.find(".html")!=string::npos || url.find(".css")!=string::npos || 
            url.find(".js")!=string::npos || url.find(".png")!=string::npos || url.find(".ico")!=string::npos){
            returnIndexPage(clientfd,url,cookie);
        }
        else if(url=="/websocket")wconnect(clientfd,header,epollfd);
        else if(url=="/register"){parseRequestBody(header,clientfd);enroll(clientfd,header);}
        else if(url=="/upload")upload(header,clientfd);
        else if(url=="/getRecord")getRecord(header,clientfd);
        else if(url.find("/login")!=string::npos)login(clientfd,url);
        else if(url.find("/getFile")!=string::npos)getFile(clientfd,url);
        else if(url.find("/download")!=string::npos)download(clientfd,header,1);
        else if(url.find("/openOnline")!=string::npos)download(clientfd,header,0);
        else if(url.find("/checkAccount")!=string::npos)checkAccount(clientfd,url);
        else if(url.find("/getTransferFile")!=string::npos)getTransferFile(clientfd,url);
        else if(url.find("/sendOnlineFile")!=string::npos)onlineTransfer(clientfd,header);
        else if(url.find("/sendCode")!=string::npos)sendCode(clientfd,url);
        else if(url.find("/resetPassword")!=string::npos)resetPassword(clientfd,url);
        else if(url.find("/test")!=string::npos)close(clientfd);
        //else std::cout<<"request:"<<header.toString()<<std::endl;
        else std::cout<<"非法路径，什么也不干"<<std::endl;
    }

public:
private:
};


















