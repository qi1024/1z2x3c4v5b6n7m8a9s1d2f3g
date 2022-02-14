#ifndef MUDUO_NET_HTTP_SQL_SERVICE_H
#define MUDUO_NET_HTTP_SQL_SERVICE_H

#include "muduo/base/Logging.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "/home//ProtocolBuffers/transfer.pb.h"
#include "/home//ProtocolBuffers/signaling.pb.h"


namespace sql{

class SqlService{
public:
  SqlService(){
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    if((clientfd=socket(AF_INET,SOCK_STREAM,0))==-1){
      errnum=errno;
      LOG_ERROR<< "cant't to create socket";
    }
    else{
      serverAddr.sin_family=AF_INET;
      serverAddr.sin_port=htons(SQL_SERVER_PORT);
      serverAddr.sin_addr.s_addr=inet_addr(SQL_SERVER_IP.c_str());
      if(connect(clientfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==-1){
        errnum=errno;
        LOG_ERROR<< "cant't to connect target server";
      }
      else LOG_INFO<<"sqlclient:connect success";
    }
  }

  ~SqlService(){/*close(clientfd);*/}
  std::string sendSql(signaling::Signaling& signal);

private:
  const std::string SQL_SERVER_IP="10.0.4.2";
  const int SQL_SERVER_PORT=2206;
  int clientfd=-1;
  int errnum;
};

}  //namespace sql


std::string sql::SqlService::sendSql(signaling::Signaling& signal){
  std::string serial;
  if(signal.SerializeToString(&serial))LOG_INFO<<"序列化成功!";
  else LOG_INFO<<"序列化失败!!";
  ssize_t len=send(clientfd,serial.c_str(),serial.size(),0);
  LOG_INFO<<"发送了:"<<len;
  const int BUFF_SIZE=1024;
  char buffer[BUFF_SIZE];
  memset(buffer,0,BUFF_SIZE);
  int readlen=0;
  std::string result;
  while((readlen=recv(clientfd,buffer,BUFF_SIZE,0))>0){
    result.append(buffer);
    memset(buffer,0,BUFF_SIZE);
  }
  return result;
}


#endif  // MUDUO_NET_HTTP_SQL_SERVICE_H

