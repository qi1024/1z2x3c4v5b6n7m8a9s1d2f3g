
//下面是muduo库版本
#include "HttpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include "postreat.hpp"
using namespace muduo;
using namespace muduo::net;

int main(int argc, char* argv[]){
  int numThreads = 0;
  int benchmark=false;
  if (argc > 1){//如果指定线程个数的话
    benchmark = true;
    Logger::setLogLevel(Logger::WARN);
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  HttpServer server(&loop, InetAddress("10.0.4.2",80), "MuduoServer");
  server.setHttpCallback(postreat);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
  return 0;
}



//g++ HttpServer.cc main.cpp -o main -I ../build/release-install-cpp11/include/ -L ../build/release-install-cpp11/lib/* -lmuduo_net -lmuduo_base -lpthread -std=c++11
//g++ HttpServer.cc main.cpp -o main -I ../build/release-install-cpp11/include/ -L ../build/release-install-cpp11/lib/* -lmuduo_net -lmuduo_base -lboost_system -lpthread -std=c++11
//g++ /home/ProtocolBuffers/signaling.pb.cc /home//ProtocolBuffers/transfer.pb.cc HttpServer.cc main.cpp -o main -I../build/release-install-cpp11/include/ -L../build/release-install-cpp11/lib/ -lmuduo_net -lmuduo_base -lboost_system -lprotobuf -lpthread -std=c++11
//g++ /home/ProtocolBuffers/signaling.pb.cc /home//ProtocolBuffers/transfer.pb.cc HttpServer.cc main.cpp -o main -I../build/release-install-cpp11/include/ -L../build/release-install-cpp11/lib/ -lmuduo_net -lmuduo_base -lboost_system -lprotobuf -lpthread -lssl -lcrypto -std=c++11