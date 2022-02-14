#pragma once
#include <string>

class Record{
public:
    Record()=default;
    ~Record()=default;

public:
    void setRecordID(const int recordID){this->recordID=recordID;}
    void setFileSize(const long long fileSize){this->fileSize=fileSize;}
    void setAccount(const std::string& account){this->account=account;}
    void setTargetAccount(const std::string& targetAccount){this->targetAccount=targetAccount;}
    void setOperation(const std::string& operation){this->operation=operation;}
    void setNickname(const std::string& nickname){this->nickname=nickname;}
    void setFileName(const std::string& fileName){this->fileName=fileName;}
    void setFileCode(const std::string& fileCode){this->fileCode=fileCode;}
    void setOperateTime(const std::string& operateTime){this->operateTime=operateTime;}

public:
    int getRecordID()const{return this->recordID;}
    std::string getAccount()const{return this->account;}
    std::string getTargetAccount()const{return this->targetAccount;}
    std::string getOperation()const{return this->operation;}
    std::string getNickname()const{return this->nickname;}
    std::string getFileName()const{return this->fileName;}
    long long getFileSize()const{return this->fileSize;}
    std::string getFileCode()const{return this->fileCode;}
    std::string getOperateTime()const{return this->operateTime;}

private:
    int recordID;
    long long fileSize;
    std::string account;
    std::string targetAccount="无";
    std::string operation;
    std::string nickname="无";
    std::string fileName;
    std::string fileCode="无";
    std::string operateTime;
};