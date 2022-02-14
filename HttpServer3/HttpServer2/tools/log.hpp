#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <cassert>
#include <iostream>
#include <ctime>
#include <unistd.h>         //getpid;
#include <sys/syscall.h>    //gettid();

/*LOG类包含如下级别*/
enum LOGLEVEL{
    LOG_LEVEL_NONE,     
    LOG_LEVEL_ERROR,    //error
    LOG_LEVEL_WARNING,  //warning
    LOG_LEVEL_DEBUG,    //debug
    LOG_LEVEL_INFO      //info
};

/*log的输出位置*/
enum LOGTARGET{
    LOG_TARGET_NONE     =0X00,
    LOG_TARGET_CONSOLE  =0X01,
    LOG_TARGET_FILE     =0x10,
    LOG_TARGET_ALL      =0x11
};

class LOG{
private:
    LOG();
    LOG(LOG& Nlog)=delete;
    LOG& operator=(LOG& nlog)=delete;
    ~LOG()=delete;
public:
    void init(LOGLEVEL loglevel,LOGTARGET logtarget);
    void destroy();
    static LOG* getInstance();

    //log级别
    LOGLEVEL getLogLevel();
    void setLogLevel(LOGLEVEL loglevel);

    //log的输出位置
    LOGTARGET getLogTarget();
    void setLogTarget(LOGTARGET logtarget);

    //LOG级别、函数所在文件名、函数名、行号、格式化、变量
    static void log(const std::string& fileName,const std::string& function,
                    const int lineNumber,const std::string& mess);

private:
    static LOG* Log;
    static std::mutex log_mutex;//互斥锁
    static std::string log_buff;//存储log的buff
    static const std::string path;
    LOGLEVEL log_level;         //log级别
    LOGTARGET log_target;       //log输出位置
    std::ofstream fout;         //文件输出流
};
/*静态变量初始化*/
LOG* LOG::Log=nullptr;
std::mutex LOG::log_mutex;
std::string LOG::log_buff="";
const std::string LOG::path="/home/huawei/project/HttpServer/log.txt";

void LOG::init(LOGLEVEL loglevel,LOGTARGET logtarget){
    setLogLevel(loglevel);
    setLogTarget(logtarget);
    fout.open(path,std::ios::app);
    if(!fout.is_open()){
        std::cerr<<"日志文件打开失败，请检查路径是否正确"<<std::endl;
        assert(fout.is_open());
    }
}

LOG::LOG(){ init(LOG_LEVEL_INFO,LOG_TARGET_ALL);}

void LOG::destroy(){
    if(fout.is_open())fout.close();
}

LOG* LOG::getInstance(){
    if(Log==nullptr){
        log_mutex.lock();
        if(Log==nullptr)Log=new LOG();
        log_mutex.unlock();
    }
    return Log;
}

LOGLEVEL LOG::getLogLevel(){return this->log_level;}

void LOG::setLogLevel(LOGLEVEL loglevel){this->log_level=loglevel;}

LOGTARGET LOG::getLogTarget(){return this->log_target;}

void LOG::setLogTarget(LOGTARGET logtarget){this->log_target=logtarget;}

void LOG::log(const std::string& fileName,const std::string& function,
                    const int lineNumber,const std::string& mess){
    time_t nowtime=time(NULL);
    tm* t=localtime(&nowtime);
    //[时间][进程号][线程号][LOG级别][文件名][函数名:行号]
    log_buff="["+std::to_string(t->tm_year+1900)+"-";
    log_buff+=std::to_string(t->tm_mon+1)+"-";
    log_buff+=std::to_string(t->tm_mday)+" ";
    log_buff+=std::to_string(t->tm_hour)+":";
    log_buff+=std::to_string(t->tm_min)+":";
    log_buff+=std::to_string(t->tm_sec)+"]";
    log_buff+="[pid:"+std::to_string(getpid())+"]";
    log_buff+="[tid:"+std::to_string(syscall(SYS_gettid))+"]";
    log_buff+="[loglevel:";

    LOGLEVEL loglevel=LOG::getInstance()->getLogLevel();
    LOGTARGET logtarget=LOG::getInstance()->getLogTarget();
    
    if(loglevel==LOG_LEVEL_DEBUG)log_buff+="debug]";
    else if(loglevel==LOG_LEVEL_INFO)log_buff+="info]";
    else if(loglevel==LOG_LEVEL_WARNING)log_buff+="warning]";
    else if(loglevel==LOG_LEVEL_ERROR)log_buff+="error]";
    log_buff+="[filename:"+fileName+"]";
    log_buff+="[function:"+function+"]";
    log_buff+="[line:"+std::to_string(lineNumber)+"]";
    log_buff+="[content:"+mess+"]";

    if(logtarget & LOG_TARGET_FILE){//10
        LOG::getInstance()->fout<<log_buff<<std::endl<<std::endl;
    }

    if(logtarget & LOG_TARGET_CONSOLE){//01
        std::cout<<log_buff<<std::endl;;
    }

    log_buff.clear();
}
