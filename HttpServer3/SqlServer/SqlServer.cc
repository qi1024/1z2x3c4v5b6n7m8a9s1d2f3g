#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/InetAddress.h"

#include"mysqlpool.h"

#include <unordered_map>
using namespace muduo;
using namespace muduo::net;

class SqlServer : noncopyable{
 public:
  SqlServer(EventLoop* loop,const InetAddress& listenAddr,int threadNum): 
  loop_(loop),server_(loop, listenAddr, "SqlServer"),mysql(MysqlPool::getMysqlPoolObject()){
    server_.setConnectionCallback(std::bind(&SqlServer::onConnection, this, _1));
    server_.setMessageCallback(std::bind(&SqlServer::onMessage,this, _1, _2, _3));
    server_.setThreadNum(threadNum);
  }

  void start(){
    mysql->setParameter("localhost","user","password","transfer",3306,NULL,0,8);
    server_.start();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn){
    if (conn->connected()){ LOG_INFO << "sqlserver:与客户端连接成功"; }
    else{ LOG_INFO << "sqlserver:与客户端连接失败";}
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time){
    string msg(buf->retrieveAllAsString());
    LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
    signaling::Signaling signal;
    if(signal.ParseFromString(msg)){
      LOG_INFO << "请求消息解析成功!command="+signal.command()+";text="+signal.text();
      mysql->execute(signal);
      std::string resmess;
      if(signal.SerializeToString(&resmess)){
        LOG_INFO << "返回消息序列化成功!";
        conn->send(resmess);
      }
      else LOG_ERROR << "返回消息序列化失败!";
    }
    else LOG_ERROR << "请求消息解析失败!";
    conn->shutdown();
    //loop_->quit();
  }

private:
  EventLoop* loop_;
  TcpServer server_;
  MysqlPool* mysql;
};

int main(int argc, char* argv[]){
  const int port = 2206;
  const int threadNum=4;
  const string ip="10.0.4.2";
  EventLoop loop;
  InetAddress serverAddr(ip,port,false);
  SqlServer server(&loop, serverAddr,threadNum);
  server.start();
  loop.loop();
  LOG_INFO << "SqlServer启动完成";
  return 0;
}


//g++ SqlServer.cc -o SqlServer -I /home/build/release-install-cpp11/include /home/build/release-install-cpp11/lib/* -lmuduo_net -lmuduo_base -lpthread -std=c++11
//g++ mysqlpool.cc SqlServer.cc -o SqlServer -I /home/build/release-install-cpp11/include /home/build/release-install-cpp11/lib/* -L/usr/lib64/mysql -lmysqlclient -lmuduo_net -lmuduo_base -lpthread -std=c++11
//g++ /home/ProtocolBuffers/transfer.pb.cc /home/ProtocolBuffers/signaling.pb.cc mysqlpool.cc SqlServer.cc -o SqlServer -I/home/build/release-install-cpp11/include -L/home/build/release-install-cpp11/lib/ -L/usr/lib64/mysql -lmysqlclient -lmuduo_net -lmuduo_base -lprotobuf -lpthread -std=c++11