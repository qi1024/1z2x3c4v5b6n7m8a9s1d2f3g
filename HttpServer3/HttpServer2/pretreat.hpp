#ifndef MUDUO_NET_HTTP_PRETREAT_H
#define MUDUO_NET_HTTP_PRETREAT_H

#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpResponse.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "midtreat.hpp"

#include <iostream>
#include <map>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

namespace muduo{
namespace net{

    class Pretreat : noncopyable{
    public:
        Pretreat(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

        void work(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

    private:
        string readFileToString(const char* filename);

        void returnPage(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);
    };


    }  // namespace net
}  // namespace muduo

    string Pretreat::readFileToString(const char* filename){
        string content;
        FILE* fp = fopen(filename, "rb");
        if (fp){
            // inefficient!!!
            const int kBufSize = 1024*1024;
            char iobuf[kBufSize];
            ::setbuffer(fp, iobuf, sizeof iobuf);

            char buf[kBufSize];
            size_t nread = 0;
            while ( (nread = fread(buf, 1, sizeof buf, fp)) > 0){
                content.append(buf, nread);
            }
            fclose(fp);
        }
        return content;
    }


    void Pretreat::returnPage(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
        Buffer buf;
        const string WEB_PATH="/home/HttpServer2/web"+req.path();
        string content=readFileToString(WEB_PATH.c_str());
      if(content.size()==0){
        res.setStatusCode(HttpResponse::k404NotFound);
        res.setStatusMessage("Not Found");
        res.setCloseConnection(true);
      }
      else{
        res.setBody(content);
        res.setStatusCode(HttpResponse::k200Ok);
        res.setStatusMessage("OK");
      }
      res.appendToBuffer(&buf);
      conn->send(&buf);
      if (res.closeConnection())conn->shutdown();
    }

    void Pretreat::work(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){

        //如果是请求页面或者是图片文件的
        if(req.path().find(".html")!=string::npos || req.path().find(".css")!=string::npos
        || req.path().find(".js")!=string::npos || req.path().find(".png")!=string::npos
        || req.path().find(".ico")!=string::npos){
            returnPage(conn,req,res);
        }
        Midtreat midtreat;

        //如果是用户表相关的请求
        if(req.path().find("user/")!=string::npos)midtreat.userClassify(conn,req,res);
        else if(req.path().find("file/")!=string::npos)midtreat.fileClassify(conn,req,res);
        else if(req.path().find("other/")!=string::npos)midtreat.otherClassify(conn,req,res);
    }

    Pretreat::Pretreat(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res){
        std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
        bool benchmark=false;
        if (!benchmark){
            const std::map<string, string>& headers = req.headers();
            for (const auto& header : headers){
              std::cout << header.first << ": " << header.second << std::endl;
            }
        }
        if(req.path()=="/" || req.path()=="/index"){req.setPath("/index.html");}
        res.addHeader("Server", "Muduo");
        if (req.path().find(".html")!=string::npos){ 
            res.setContentType("text/html; charset=utf-8"); 
        }
        else if (req.path().find(".css")!=string::npos){
            res.setContentType("text/css; charset=utf-8");
        }
        else if (req.path().find(".js")!=string::npos){
            res.setContentType("application/x-javascript; charset=utf-8");
        }
        else if (req.path().find(".png")!=string::npos){
            res.setContentType("image/png");
        }
        else if (req.path().find(".ico")!=string::npos){
            res.setContentType("image/x-icon");
        }
        /*else{
            res.setStatusCode(HttpResponse::k404NotFound);
            res.setStatusMessage("Not Found");
            res.setCloseConnection(true);
        }*/
    }

#endif  // MUDUO_NET_HTTP_PRETREAT_H
