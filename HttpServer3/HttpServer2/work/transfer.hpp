#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
# include <climits>
#include "../projo/UserFile.hpp"
#include "../projo/RequestHeader.hpp"
#include "../projo/ResponseHeader.hpp"
#include "../projo/record.hpp"
#include "../tools/newrand.hpp"
#include "../sql/mysql_service.hpp"

void upload(RequestHeader& header,int clientfd){
    using std::string;
    string contentType="";
    if(header.getFields("Content-Type",contentType)==0)return;
    string::size_type pos=contentType.find("=");
    string boundary=string(contentType.begin()+pos+1,contentType.end());
    //std::cout<<"boundary:"<<boundary<<std::endl;
    string line,fileName,fileType;
    string fileCode,filePath;
    long long fileSize;
    char c;
    unsigned READ_BUFFER_SIZE=INT_MAX; 
    char* buffer=new char[READ_BUFFER_SIZE];
    while(read(clientfd,&c,1)>0){
        if(c!='\r')line.push_back(c);
        else if(read(clientfd,&c,1)>0 && c=='\n'){
            line.push_back('\n');
            if(line.find(boundary)!=string::npos){line.clear();continue;}
            else if(line.find("filesize")!=string::npos){
                read(clientfd,&c,1);//舍弃'\r'
                read(clientfd,&c,1);//舍弃'\n'
                line.clear();
                while(read(clientfd,&c,1)>0 && c!='\r')line.push_back(c);
                read(clientfd,&c,1);//舍弃'\n'
                fileSize=atol(line.c_str());
            }
            else if((pos=line.find("filename="))!=string::npos){
                fileName=string(line.begin()+pos+10,line.end());
                fileName.pop_back();
                fileName.pop_back();
            }
            else if((pos=line.find("Content-Type"))!=string::npos){
                fileType=string(line.begin()+14,line.end());
                read(clientfd,&c,1);//舍弃\r
                read(clientfd,&c,1);//舍弃\n
                newrand nrand;
                fileCode=nrand.nextDigitAndLetter(20);
                string WEB_PATH="/home/huawei/project/HttpServer2/web/download/";
                filePath=WEB_PATH+fileCode;
                UserFile userfile(fileSize,fileCode,fileName,filePath);
                Mysql_service service;
                while(service.addFile(userfile)<1){
                    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"文件信息添加失败！errno:"+
                    std::to_string(errno)+":"+strerror(errno));
                    fileCode=nrand.nextDigitAndLetter(20);
                    filePath=WEB_PATH+fileCode;//文件提取码可能重复
                    userfile.setFileCode(fileCode);
                    userfile.setFilePath(filePath);
                }

                string cookie;
                header.getFields("Cookie",cookie);
                string::size_type pos1=cookie.find("account=");
                string::size_type pos2=cookie.find("&password=");
                if(pos1!=string::npos){//说明用户处于登录状态，要记录上传记录
                    string account(cookie.begin()+pos1+8,cookie.begin()+pos2);
                    //string password(cookie.begin()+pos2+10,cookie.end());
                    Record record;
                    record.setAccount(account);
                    record.setOperation("上传");
                    record.setFileName(fileName);
                    record.setFileSize(fileSize);
                    record.setFileCode(fileCode);
                    if(service.addRecord(record)<1){
                        LOG::getInstance()->log(__FILE__,__func__,__LINE__,"上传记录添加失败！errno:"+
                        std::to_string(errno)+":"+strerror(errno));
                    }
                }

                //下面这段代码是之前创建文件夹用，现在已舍弃
                /*string dirpath=WEB_PATH+"/download/"+fileCode;
                if(mkdir(dirpath.c_str(),S_IRWXU)<0){
                    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"目录创建失败！");
                    service.deleteFile(fileCode);
                    break;
                }
                else{*/

                /*下面这段代码将文件写入服务器*/
                FILE* file=fopen(filePath.c_str(),"wb+");
                if(file==nullptr){
                    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"文件创建失败！");
                    service.deleteFile(fileCode);
                    break;
                }
                else{
                    int read_bytes=-1;
                    int write_bytes=-1;
                    int res_bytes=-1;
                    while(fileSize && (read_bytes=read(clientfd,buffer,READ_BUFFER_SIZE))>0){
                        if(fileSize>read_bytes){
                            write_bytes=fwrite(buffer,1,read_bytes,file);
                            res_bytes=read_bytes-write_bytes;
                            while(res_bytes>0)res_bytes-=fwrite(buffer,1,res_bytes,file);
                            fileSize-=read_bytes;
                        }
                        else {
                            write_bytes=fwrite(buffer,1,fileSize,file);
                            res_bytes=fileSize-write_bytes;
                            while(res_bytes>0)res_bytes-=fwrite(buffer,1,res_bytes,file);
                            break;
                        }
                    }
                    fclose(file);
                    
                    /*下面这段代码登记文件上传操作*/
                    string cookie;
                    header.getFields("Cookie",cookie);
                    string::size_type pos1=cookie.find("account=");
                    string::size_type pos2=cookie.find("&password=");
                    if(pos1!=string::npos){//说明用户处于登录状态，要记录上传记录
                        string account(cookie.begin()+pos1+8,cookie.begin()+pos2);
                        //string password(cookie.begin()+pos2+10,cookie.end());
                        Record record;
                        record.setAccount(account);
                        record.setOperation("上传");
                        record.setFileName(fileName);
                        record.setFileSize(fileSize);
                        record.setFileCode(fileCode);
                        if(service.addRecord(record)<1){
                            LOG::getInstance()->log(__FILE__,__func__,__LINE__,"上传记录添加失败！errno:"+
                            std::to_string(errno)+":"+strerror(errno));
                        }
                    }

                    ResponseHeader header("HTTP/1.1",200);
                    header.setField("Content-Type","text/plain; charset=UTF-8");
                    time_t tt;
                    unsigned timeStamp=time(&tt)+24*60*60;
                    header.addJson("result","200");
                    header.addJson("time",std::to_string(timeStamp));
                    header.addJson("fileCode",fileCode);
                    string json;
                    header.setField("Content-Length",std::to_string(header.toJson(json)));
                    string result=header.toString()+json;
                    send(clientfd,result.c_str(),result.size(),0);
                }
                
            }
            line.clear();
        }  
    }
    delete[] buffer;
    close(clientfd);
}




void getFile(int clientfd,const std::string& url){
    using std::string;
    string::size_type pos=url.find("fileCode=");
    if(pos==string::npos)return;
    string fileCode=string(url.begin()+pos+9,url.end());
    if(fileCode.size()!=20)return;
    UserFile file(fileCode);
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    Mysql_service service;
    if(service.getFile(file)>0){
        header.setStatusCode(200);
        header.addJson("fileName",file.getFileName());
        header.addJson("fileSize",std::to_string(file.getFileSize()));
        header.addJson("fileCode",file.getFileCode());
        header.addJson("uploadTime",file.getUploadTime());
        string json;
        header.setField("Content-Length",std::to_string(header.toJson(json)));
        string result=header.toString()+json;
        send(clientfd,result.c_str(),result.size(),0);
    }
    else {
        header.setStatusCode(404);
        header.setField("Content-Length","0");
        string result=header.toString();
        send(clientfd,result.c_str(),result.size(),0);
    }
    close(clientfd);
}


void download(int clientfd,RequestHeader& request,int flag){
    using std::string;
    string url=request.getUrl();
    string::size_type pos=url.find("fileCode=");
    if(pos==string::npos)return;
    string fileCode=string(url.begin()+pos+9,url.end());
    UserFile file(fileCode);
    ResponseHeader header;
    header.setVersion("HTTP/1.1");
    Mysql_service service;
    if(service.getFile(file)>0){
        int filefd = open(file.getFilePath().c_str(),O_RDONLY|O_LARGEFILE);
        if(filefd<0){close(filefd);return;}
        long long fileSize=file.getFileSize();
        header.setStatusCode(200);
        header.setField("Content-Type","application/octet-stream");
        header.setField("Content-Disposition","attachment; filename=\""+file.getFileName()+"\"");
        header.setField("Content-Length",std::to_string(fileSize));
        header.setField("Server","HttpServer,ubuntu(debian)");
        header.setField("Content-Transfer-Encoding","binary");
        header.setField("Connection","keep-alive");
        string result=header.toString();
        send(clientfd,result.c_str(),result.size(),0);
        
        if(fileSize>INT_MAX){
            const int BUFFER_SIZE=INT_MAX;
            char* buffer=new char[BUFFER_SIZE];
            unsigned readSize=0;
            while(fileSize>0){
                if(fileSize>INT_MAX)readSize=read(filefd,buffer,BUFFER_SIZE);
                else readSize=read(filefd,buffer,fileSize);
                fileSize-=readSize;
                while((readSize-=(send(clientfd,buffer,readSize,0)))>0);
            }
            delete[] buffer;
        }
        else sendfile(clientfd,filefd,nullptr,fileSize);
        close(filefd);
    }
    else std::cout<<"出现未知错误"<<std::endl;

    if(flag){
        string cookie;
        request.getFields("Cookie",cookie);
        string::size_type pos1=cookie.find("account=");
        string::size_type pos2=cookie.find("&password=");
        if(pos1!=string::npos){//说明用户处于登录状态，要记录上传记录
            string account(cookie.begin()+pos1+8,cookie.begin()+pos2);
            //string password(cookie.begin()+pos2+10,cookie.end());
            Record record;
            record.setAccount(account);
            record.setOperation("下载");
            record.setFileName(file.getFileName());
            record.setFileSize(file.getFileSize());
            record.setFileCode(fileCode);
            if(service.addRecord(record)<1){
                LOG::getInstance()->log(__FILE__,__func__,__LINE__,"下载记录添加失败！errno:"+
                std::to_string(errno)+":"+strerror(errno));
            }
        }
    }

    close(clientfd);
}

void getTransferFile(int clientfd,const std::string& url){
    std::string::size_type pos=url.find("fileKey=");
    std::string fileKey(url.begin()+pos+8,url.end());
    int ret=Wsconnect::addConnection(fileKey,clientfd);
}

void onlineTransfer(int clientfd,RequestHeader& request){
    using std::string;

    string url=request.getUrl();
    string::size_type pos1=url.find("targetAccount=");
    string::size_type pos2=url.find("&fileSize=");
    string::size_type pos3=url.find("&fileKey=");
    string targetAccount(url.begin()+pos1+14,url.begin()+pos2);
    string fileSize(url.begin()+pos2+10,url.begin()+pos3);
    string fileKey(url.begin()+pos3+9,url.end());
    string line,fileName;
    char c;
    string::size_type pos=string::npos;
    while(read(clientfd,&c,1)>0){
        if(c!='\r')line.push_back(c);
        else {
            read(clientfd,&c,1);//舍弃'\n'
            pos=line.find("filename=\"");
            if(pos!=string::npos){
                fileName=string(line.begin()+pos+10,line.end());
                fileName.pop_back();
            }
            line.clear();
            if(read(clientfd,&c,1) && c=='\r'){
                read(clientfd,&c,1);//舍弃'\n'
                break;
            }
            else line.push_back(c);
        }
    }
    
    ResponseHeader response;
    response.setVersion("HTTP/1.1");
    response.setStatusCode(200);
    response.setField("Content-Type","application/octet-stream");
    response.setField("Content-Disposition","attachment; filename=\""+fileName+"\"");
    response.setField("Content-Length",fileSize);
    response.setField("Server","HttpServer,ubuntu(debian)");
    response.setField("Content-Transfer-Encoding","binary");
    response.setField("Connection","keep-alive");
    string result=response.toString();

    int targetFd=Wsconnect::getConection(fileKey);

    send(targetFd,result.c_str(),result.size(),0);

    long long size=atoi(fileSize.c_str());
    const int bufferSize=1024*1024;
    int length=0;
    char temp[bufferSize]={0};
    memset(temp,'\0',bufferSize);

    while(size>0){
        length=read(clientfd,temp,bufferSize);
        if(size>length)send(targetFd,temp,length,0);
        else send(targetFd,temp,size,0);
        size-=length;
        memset(temp,'\0',bufferSize);
    }
    //std::cout<<"文件传输完成"<<std::endl;
    Wsconnect::deleteConnection(fileKey);

    ResponseHeader respo2("HTTP/1.1",200);
    respo2.setField("Content-Type","text/plain; charset=UTF-8");
    string result2=respo2.toString();
    send(clientfd,result2.c_str(),result2.size(),0);

    string cookie;
    request.getFields("Cookie",cookie);
    pos1=cookie.find("account=");
    pos2=cookie.find("&password=");
    string account(cookie.begin()+pos1+8,cookie.begin()+pos2);

    Record record;
    record.setAccount(account);
    record.setOperation("在线上传");
    record.setFileName(fileName);
    record.setFileSize(atoi(fileSize.c_str()));
    record.setTargetAccount(targetAccount);
    record.setNickname(Users::getNickname(targetAccount));
    
    Mysql_service service;
    if(service.addRecord(record)<1){
        LOG::getInstance()->log(__FILE__,__func__,__LINE__,"在线上传记录添加失败！errno:"+
        std::to_string(errno)+":"+strerror(errno));
    }

    record.setAccount(targetAccount);
    record.setOperation("在线下载");
    record.setTargetAccount(account);
    record.setNickname(Users::getNickname(account));
    if(service.addRecord(record)<1){
        LOG::getInstance()->log(__FILE__,__func__,__LINE__,"在线下载记录添加失败！errno:"+
        std::to_string(errno)+":"+strerror(errno));
    }

    close(clientfd);
    close(targetFd);
}


void getRecord(RequestHeader& request,int clientfd){
    using std::string;
    string cookie;
    request.getFields("Cookie",cookie);
    string::size_type pos1=cookie.find("account=");
    string::size_type pos2=cookie.find("&password=");

    if(pos1!=string::npos){
        string account(cookie.begin()+pos1+8,cookie.begin()+pos2);
        ResponseHeader header;
        header.setVersion("HTTP/1.1");
        Mysql_service service;
        std::vector<Record>records;
        string result;
        header.setStatusCode(200);
        if(service.getRecord(account,records)>0 && records.size()>0){
            string jsons="[";
            for(int i=0;i<records.size();++i){
                jsons.push_back('{');
                jsons+="\"operation\":\""+records[i].getOperation()+"\",";
                jsons+="\"fileName\":\""+records[i].getFileName()+"\",";
                jsons+="\"fileSize\":\""+std::to_string(records[i].getFileSize())+"\",";
                jsons+="\"fileCode\":\""+records[i].getFileCode()+"\",";
                jsons+="\"targetAccount\":\""+records[i].getTargetAccount()+"\",";
                jsons+="\"nickname\":\""+records[i].getNickname()+"\",";
                jsons+="\"operateTime\":\""+records[i].getOperateTime();
                jsons+="\"},";
            }
            jsons[jsons.size()-1]=']';
            header.setField("Content-Length",std::to_string(jsons.size()));
            //std::cout<<"records.size():"<<records.size()<<std::endl;
            result=header.toString();
            send(clientfd,result.c_str(),result.size(),0);
            send(clientfd,jsons.c_str(),jsons.size(),0);
        
        }
        else {
           header.setField("Content-Length","0");
           result=header.toString();
           send(clientfd,result.c_str(),result.size(),0);
        }
    }

    
    close(clientfd);
}