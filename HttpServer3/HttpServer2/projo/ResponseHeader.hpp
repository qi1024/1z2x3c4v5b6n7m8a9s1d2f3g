#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class ResponseHeader{

public:
    ResponseHeader(){}
    ResponseHeader(const std::string& ver,int code):version(ver),status_code(code){}
    ~ResponseHeader(){}

public:
    void setField(const std::string& key,const std::string& value){fields[key]=value;}
    void setVersion(const std::string& ver){this->version=ver;}
    void setStatusCode(int code){this->status_code=code;}
    void setFd(int fd){this->fd=fd;}

    int getFd()const{return this->fd;}

    int getField(const std::string& key,std::string& value){
        if(fields.count(key)){value=fields[key];return 1;}
        else return 0;
    }
    int removeField(const std::string& key){
        if(fields.count(key)){fields.erase(key);return 1;}
        else return 0;
    }
    void addJson(const std::string& key,const std::string& value){json_map[key]=value;}

    std::string toString(){
        std::string status="";
        if(status_mess.count(status_code))status=status_mess.at(status_code);
        std::string result=version+" "+std::to_string(status_code)+" "+status+"\r\n";
        for(auto& word:fields)result+=word.first+": "+word.second+"\r\n";
        result+="\r\n";
        return result;
    }

    int toJson(std::string& result){
        result.clear();
        if(json_map.size()>0){
            result+="{";
            for(auto& word:json_map)result+="\""+word.first+"\":\""+word.second+"\",";
            result.pop_back();
            result+="}";
        }
        return result.size();
    }

private:
    std::string version="HTTP/1.1";
    int status_code=200;
    int fd=-1;
    const static std::unordered_map<int,std::string>status_mess;
    std::unordered_map<std::string,std::string>fields{
        {"Content-Type","text/plain; charset=UTF-8"},
        {"Content-Encoding","UTF-8"},
        {"Server","HttpServer(debian/ubuntu)"},
        {"Connection","close"},
        {"Accept-Ranges","bytes"},
        {"Content-Length","0"},
        {"Content-Language","zh-CN, en-US"}
        /*{"Transfer-Encoding","identity"}*/
    };
    std::unordered_map<std::string,std::string>json_map;
    //std::vector<std::unordered_map<std::string,std::string>>jsons;
};
const std::unordered_map<int,std::string> ResponseHeader::status_mess{
    {101,"Switching Protocols"},
    {200,"OK"},{201,"Created"},{301,"Moved Permanently"},{302,"Found"},
    {304,"Not Modified"},{403,"Forbidden"},{404,"Not Found"},{405,"Method Not Allowed"},
    {500,"Internal Server Error"},{502,"Bad Gateway"},{503,"Service Unavailable"},
    {505,"HTTP Version Not Supported"}
};
