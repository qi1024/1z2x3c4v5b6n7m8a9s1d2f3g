#ifndef MUDUO_NET_HTTP_HANDLER_REQUEST_H
#define MUDUO_NET_HTTP_HANDLER_REQUEST_H

#include "muduo/net/http/HttpRequest.h"
#include "muduo/net/http/HttpResponse.h"
#include "muduo/net/TcpServer.h"

namespace muduo{
namespace net{

class HandlerRequest : noncopyable{
 public:
  
  HandlerRequest(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

  void work();

  private:
  string readFileToString(const char* filename);
  void returnPage(const TcpConnectionPtr& conn,HttpRequest& req,HttpResponse& res);

};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_HTTP_HTTPSERVER_H
