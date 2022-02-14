#pragma once
#include "WebSocketService.hpp"


class WebSocket{
private:
    WebSocket()=delete;
    ~WebSocket(){}

public:
    static void parWsRequestHeader(int clientfd,WsRequestHeader& header){
        std::stringstream ssline;
        std::string line,key,value;
        char temp[256]={0};
        int length=0;
        std::string::size_type pos=-1;
        std::string message,last;

        if((length=recv(clientfd,temp,256,0))>0){
            unPackingWSFrameData(temp,length,message);
            pos=message.find("\n\n");
            //std::cout<<"pos:"<<pos<<std::endl;
            if(pos!=std::string::npos){
                std::string head(message.begin(),message.begin()+pos);
                last=std::string(message.begin()+pos+2,message.end());
                ssline<<head;
                while(ssline>>key>>value)header.addParameter(key,value);
            }
        }
    }

public:

    static void wsprocess(int clientfd){
        using std::string;
        WsRequestHeader header;
        parWsRequestHeader(clientfd,header);
        std::string command=header.getParameter("command");
        if(command=="onlineRequest")onlineRequest(clientfd,header);
        else if(command=="onlineResponse")onlineResponse(clientfd,header);
    }
    

private:
    static pthread_mutex_t mutex;
    static std::unordered_map<std::string,User>users;
};

pthread_mutex_t WebSocket::mutex=PTHREAD_MUTEX_INITIALIZER;
std::unordered_map<std::string,User> WebSocket::users;