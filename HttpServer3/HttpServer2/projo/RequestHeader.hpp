#pragma once
#include <string>
#include <sstream>
#include <unordered_map>

/*HTTP请求头=请求行+头部字段*/
class RequestHeader{
public:
    RequestHeader(){}
    RequestHeader(const std::string& p_meod,const std::string& p_url,const std::string& p_version):
        method(p_meod),url(p_url),version(p_version){}
    ~RequestHeader(){}

public:
    void setMethod(const std::string& p_method){this->method=p_method;}
    void setUrl(const std::string& p_url){this->url=p_url;}
    void setVersion(const std::string& p_version){this->version=p_version;}
    void setResStr(const std::string& str){this->res_str=str;}
    std::string getMethod(){return this->method;}
    std::string getUrl(){return this->url;}
    std::string getVersion(){return this->method;}
    std::string getResStr(){return this->res_str;}

public:
    void addFields(const std::string& key,const std::string& value){fields[key]=value;}

    int getFields(const std::string& key,std::string& value){
        if(fields.count(key)){ value=fields[key]; return 1;}
        else{ value=""; return 0;}
    }

    void parseParam(const char* param){
        using std::string;
        if(param=="")return;
        std::stringstream ss(param);
        string word,key,value;
        while(getline(ss,word,'&')){
            string::size_type pos=word.find_first_of("=");
            if(pos!=string::npos){
                string key(word.begin(),word.begin()+pos);
                string value(word.begin()+pos+1,word.end());
                if(key!=" " && value!=" ")addParameter(key,value);
            }
        }
        ss.clear();
    }

    std::string getParameter(const std::string& key){
        if(parameters.count(key))return parameters[key];
        else return "";
    }

    void addParameter(const std::string& key,const std::string& value){parameters[key]=value;}

public:
    std::string toString(){
        std::string result="method:"+method+"\n";
        result+="url:"+url+"\n";
        result+="version:"+version+"\n";
        for(auto& elem:fields)result+=elem.first+":"+elem.second+"\n";
        if(parameters.size()>0){
            result+="parameters:[\n";
            for(auto& param:parameters)result+=param.first+"="+param.second+"\n";
            result+="]\n";
        }
        return result;
    }    

private:
    std::string method="";
    std::string url="";
    std::string version="";
    std::string res_str="";
    std::unordered_map<std::string,std::string>fields;      //保存头部字段
    std::unordered_map<std::string,std::string>parameters; //保存请求参数
};
