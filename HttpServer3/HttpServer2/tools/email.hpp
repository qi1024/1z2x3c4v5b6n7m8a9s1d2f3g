#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include "encrypt.hpp"


class Email{

public:
    Email()=default;

public:

int sendEmail( const std::string& dest_account, const std::string& content){
    std::string smtp_url;
    int smtp_port = 25;

    //获取smtp服务器地址
    std::string::size_type pos = user_account.find('@');
    if (pos != std::string::npos){
        smtp_url = "smtp." + user_account.substr(pos + 1);
    }
    else{
        std::cout << "get smtp_url error, user_account:" << user_account << std::endl;
        return -1;
    }

    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    hostent* he = ::gethostbyname(smtp_url.c_str());
    if (he == NULL){
        std::cout << "get host error, smtp_url:" << smtp_url << std::endl;
        return -1;
    }


    struct sockaddr_in addr;
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(smtp_port);

    if (::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        std::cout << "connect error" << std::endl;
        return -1;
    }

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0){
        std::cout << "set recv timeout error" << std::endl;
        return 0;
    }

    if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0){
        std::cout << "set send timeout error" << std::endl;
        return 0;
    }

    char buff[1024] = {0};
    size_t len = sizeof(buff);    

    //step 1: connect
    int recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(1);
    
    //step 2: EHLO
    std::string send("EHLO ");
    send += smtp_url;
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(2);

    //step 3: AUTH LOGIN
    send = "AUTH LOGIN\r\n";
    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
   // SMTP_LOG(3);

    //step 4: account_name
    std::string account_name;
    pos = user_account.find("@");
    if (pos != std::string::npos){ account_name = user_account.substr(0, pos);}

    //send = TC_Base64::encode(account_name);
    send=codec::base64_encode(account_name.c_str(),account_name.size());
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(4);

    //step 5: account_pwd
    //send = TC_Base64::encode(user_passwd);
    send=codec::base64_encode(user_passwd.c_str(),user_passwd.size());
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(5);

    if (std::string(buff, recv_len).find("235") == std::string::npos){
        std::cout << "account password error" << std::endl;
        return -1;
    }

    //step 6: MAIL FROM
    send = "MAIL FROM: ";
    send += "<" + user_account + ">";
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(6);

    //step 7: RCPT TO
    /*for (size_t i = 0; i != dest_account.size(); ++i){
        send = "RCPT TO: ";
        send += "<" + dest_account[i] + ">";
        send += "\r\n";
        ::send(sock, send.data(), send.size(), 0);
        recv_len = ::recv(sock, buff, len, 0);
        //SMTP_LOG(7);
    }*/

    send = "RCPT TO: ";
    send += "<" + dest_account + ">";
    send += "\r\n";
    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);

    //step 8: DATA
    send = "DATA";
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(8);

    //step 9: content
    send = "";
    send += "FROM: " + user_account;
    send += "\r\n";

    send += "TO: ";
    /*for (size_t i = 0; i != dest_account.size(); ++i){
        if (i != 0)send += ",";
        send += dest_account[i];
    }*/
    send +=dest_account;
    send += "\r\n";
    send += "SUBJECT: 验证码";
    send += "\r\n\r\n";
    send += content;
    send += "\r\n\r\n";
    send += ".";
    send += "\r\n";

    ::send(sock, send.data(), send.size(), 0);
    recv_len = ::recv(sock, buff, len, 0);
    //SMTP_LOG(9);

    return 0;
}

private:
    const std::string user_account="1106465251@qq.com"; 
    const std::string user_passwd="ugtpxvifiejjbadc";

};
//"ugtpxvifiejjbadc"
//"bhlisjgadhndhgei"