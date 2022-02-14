#pragma once
#include <unordered_map>

class Wsconnect{
public:
    Wsconnect()=default;
    ~Wsconnect()=default;

public:
    static int getConection(const std::string& fileKey){
        if(connections.count(fileKey)) return connections[fileKey];
        else return -1;
    }

    static int addConnection(const std::string& fileKey,int clientfd){
        int result=1;
        pthread_mutex_lock(&mutex);
        if(connections.count(fileKey))result=-1;
        else {
            connections[fileKey]=clientfd;
            result=1;
        }
        pthread_mutex_unlock(&mutex);
        return result;
    }

    static void deleteConnection(const std::string& fileKey){
        pthread_mutex_lock(&mutex);
        if(connections.count(fileKey))connections.erase(fileKey);
        pthread_mutex_unlock(&mutex);
    }

private:
    static pthread_mutex_t mutex;
    static std::unordered_map<std::string,int> connections;
};

std::unordered_map<std::string,int> Wsconnect::connections;
pthread_mutex_t Wsconnect::mutex=PTHREAD_MUTEX_INITIALIZER;