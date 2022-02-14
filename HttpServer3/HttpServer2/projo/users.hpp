#pragma once
#include "User.hpp"
#include <unordered_map>

class Users{
public:
    Users()=default;
    ~Users()=default;

public:
    static void addUser(const User& user){
        pthread_mutex_lock(&mutex);
        users[user.getAccount()]=user;
        pthread_mutex_unlock(&mutex);
    }

    static bool checkUser(const std::string&account,const std::string& password){
        if(users.count(account) && users[account].getPassword()==password){
            return true;
        }
        else return false;
    }

    static bool checkUser(const std::string&account){return users.count(account);}

    static std::string getPassword(const std::string& account){
        if(users.count(account))return users[account].getPassword();
        else return "";
    }

    static std::string getNickname(const std::string& account){
        if(users.count(account))return users[account].getNickname();
        else return "";
    }

    static int getFd(const std::string& account){
        if(users.count(account))return users[account].getFd();
        else return -1;
    }
    

private:
    static pthread_mutex_t mutex;
    static std::unordered_map<std::string,User>users;
};

std::unordered_map<std::string,User> Users::users;
pthread_mutex_t Users::mutex=PTHREAD_MUTEX_INITIALIZER;