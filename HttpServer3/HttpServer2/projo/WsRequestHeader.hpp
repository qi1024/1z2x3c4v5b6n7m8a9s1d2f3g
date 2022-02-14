#pragma once
#include <string>
#include <unordered_map>

class WsRequestHeader{
public:
    WsRequestHeader(){}
    ~WsRequestHeader(){}

public:
    void addParameter(const std::string& key,const std::string& value){parameters[key]=value;}

    std::string getParameter(const std::string& key){
        if(parameters.count(key))return parameters[key];
        else return "";
    }

public:
    std::string toString(){
        std::string result="";
        if(parameters.size()>0){
            for(auto& param:parameters)result+=param.first+":"+param.second+"\n";
        }
        return result;
    }    

private:
    std::unordered_map<std::string,std::string>parameters; //保存请求参数
};
