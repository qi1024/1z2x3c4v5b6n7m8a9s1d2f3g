#pragma once
#include <string>

class User{
public:
    User()=default;
    User(int userID,const std::string& account,const std::string& nickname):
        userID(userID),account(account),nickname(nickname){}
    ~User()=default;


public:
    void setUserID(int userID){this->userID=userID;}
    void setFd(int fd){this->fd=fd;}
    void setAccount(const std::string& account){this->account=account;}
    void setPassword(const std::string& password){this->password=password;}
    void setStatus(const std::string& status){this->status=status;}
    void setNickname(const std::string& nickname){this->nickname=nickname;}
    void setCreateTime(const std::string& createTIme){this->createTime=createTime;}

public:
    int getUserID()const{return this->userID;}
    int getFd()const{return this->fd;}
    std::string getAccount()const{return this->account;}
    std::string getPassword()const{return this->password;}
    std::string getStatus()const{return this->status;}
    std::string getNickname()const{return this->nickname;}
    std::string getCreateTime()const{return this->createTime;}


private:
    int userID;
    int fd;
    std::string account;
    std::string password;
    std::string status;
    std::string nickname;
    std::string createTime;
};
