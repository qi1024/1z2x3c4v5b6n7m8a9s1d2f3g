#pragma once
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "../tools/log.hpp"
#include "thread_pool.hpp"
#include "httpwork_wrapper.hpp"
#include "WebSocketWrapper.hpp"

class HttpServer {
private:
	HttpServer() {
		int ret = 0;
		bzero(&http_addr, sizeof(http_addr));
		http_addr.sin_family = AF_INET;
		http_addr.sin_addr.s_addr=inet_addr( http_ip.c_str());
		http_addr.sin_port = htons(http_port);
		httpfd = socket(PF_INET, SOCK_STREAM, 0);
		if (httpfd < 0) { 
			LOG::getInstance()->log(__FILE__,__func__,__LINE__,"http socket创建失败");
			assert(false);
		}
		else LOG::getInstance()->log(__FILE__,__func__,__LINE__,"http socket创建成功");
		ret = bind(httpfd, (struct sockaddr*)&http_addr, sizeof(http_addr));
		if (ret == -1) { 
			LOG::getInstance()->log(__FILE__,__func__,__LINE__,strerror(errno));
			assert(false);
		}
		else LOG::getInstance()->log(__FILE__,__func__,__LINE__,"http socket绑定成功");
		ret = listen(httpfd, CONN_NUM);
		if (ret == -1) {
			LOG::getInstance()->log(__FILE__,__func__,__LINE__,strerror(errno));
			assert(false);
		}
		else LOG::getInstance()->log(__FILE__,__func__,__LINE__,"http socket监听成功");

		if(fcntl(httpfd,F_SETFL,O_NONBLOCK)!=-1){//一定要设置成非阻塞模式
			LOG::getInstance()->log(__FILE__,__func__,__LINE__,"非阻塞模式设置成功");
		}

		epoll_handle=epoll_create(CONN_NUM);
		if(epoll_handle<0){
			LOG::getInstance()->log(__FILE__,__func__,__LINE__,"epoll句柄创建失败");
			return;
		}
	}
	
public:
	~HttpServer(){}

	static HttpServer* getInstance() {
		if (server == nullptr) {
			 pthread_mutex_lock(&mutex);
			if (server == nullptr)server = new HttpServer();
			 pthread_mutex_unlock(&mutex);
		}
		return server;
	}
	
	void run(){
		std::ios::sync_with_stdio(false);
		LOG::getInstance()->log(__FILE__,__func__,__LINE__,"HTTP服务器启动完成");
		struct sockaddr_in caddress;
		socklen_t caddress_length=0;
		int connfd=-1;
		int fdnum=-1;

		while(true){
			//下面是http请求
			caddress_length= sizeof(caddress);
			connfd = accept(httpfd, (struct sockaddr*)&caddress, &caddress_length);
			if(connfd>0){
				LOG::getInstance()->log(__FILE__,__func__,__LINE__,"第"+std::to_string(++http_num)+"http连接成功");
				httpwork_wrapper service(epoll_handle,connfd);
				while(thread_pool<httpwork_wrapper>::getInstance()->append(service)==0);
			}

			//下面是WebSocket
			fdnum=epoll_wait(epoll_handle,events,CONN_NUM,0);
			for(int i=0;i<fdnum;++i){
				std::cout<<"收到WebSocket请求"<<std::endl;
				WebSocketWrapper wrapper(events[i].data.fd);
				while(thread_pool<WebSocketWrapper>::getInstance()->append(wrapper)==0);
			}
			fdnum=-1;


		}
	}

private:
	static HttpServer* server;
	static int httpfd;
	static pthread_mutex_t mutex;/*互斥锁*/
private:
	const std::string http_ip = "192.168.31.170";
	const int http_port = 9090;  
	const int CONN_NUM=128;
	unsigned long long http_num=0;
	struct sockaddr_in http_addr;
	struct epoll_event events[128];
	int epoll_handle=-1;
	
};

int HttpServer::httpfd=-1;
HttpServer* HttpServer::server=nullptr;
pthread_mutex_t HttpServer::mutex=PTHREAD_MUTEX_INITIALIZER;

