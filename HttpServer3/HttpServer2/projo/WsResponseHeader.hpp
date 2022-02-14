#pragma once
#include <string>
#include <unordered_map>

class WsResponseHeader{
public:
    WsResponseHeader(){}
    ~WsResponseHeader(){}

public:
    void addParameter(const std::string& key,const std::string& value){parameters[key]=value;}

    std::string getParameter(const std::string& key){
        if(parameters.count(key))return parameters[key];
        else return "";
    }

    std::string toJson(){
        std::string result="";
        if(parameters.size()>0){
            result+="{";
            for(auto& word:parameters)result+="\""+word.first+"\":\""+word.second+"\",";
            result.back()='}';
        }
        return result;
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
