#pragma once
#include <cstring>
#include <string>
#include <openssl/sha.h>

namespace codec{

inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}
                                               //A\nB                          3
std::string base64_encode(char const* bytes_to_encode, int in_len) {
    const std::string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(i = 0; (i < 4) ; i++)
        ret += base64_chars[char_array_4[i]];
        i = 0;
    }
    }

    if (i)
    {
    for(j = i; j < 3; j++)
        char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
        ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
        ret += '=';

    }

  return ret;

}

int SHA1_encrypt(const char *str,int len,char *out_data){
	unsigned char md[SHA_DIGEST_LENGTH];
	char tmp[3] = {'\0'};
	char *buf =out_data;// (char*)malloc(33);
	//bzero(buf,33);
	SHA1((unsigned char*)str,strlen((char*)str),md);
	int i = 0;
	for(i=0;i<SHA_DIGEST_LENGTH;i++){
		sprintf(tmp,"%2.2x",md[i]);
		strcat(buf,tmp);
	}
	return 0;
}

int htoi(const char s[],int start,int len) { 
    int i,j; 
    int n = 0; 
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X')) i = 2; 
    else i = 0; 
    i+=start;
    j=0;
    for (; (s[i] >= '0' && s[i] <= '9') 
	   || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >='A' && s[i] <= 'F');++i) {   

        if(j>=len)break;
        if (tolower(s[i]) > '9') n = 16 * n + (10 + tolower(s[i]) - 'a'); 
        else n = 16 * n + (tolower(s[i]) - '0'); 
	    j++;
    } 
    return n; 
} 


int unPackingWSFrameData(const char *msg,int msgLen,std::string &outBuf){
    //报文长度一定大于2字节，对于小于的，做返回处理
    if(msgLen < 2) return -3;
    uint8_t opcode_ = 0;
    uint8_t mask_ = 0;
    uint8_t masking_key_[4] = {0,0,0,0};
    uint64_t payload_length_ = 0;
    int pos = 0;
 
    //Opcode
    opcode_ = msg[pos] & 0x0f;
    pos++;
    //MASK
    mask_ = (unsigned char)msg[pos] >> 7;
    //Payload length
    payload_length_ = msg[pos] & 0x7f;
    pos++;
    if(payload_length_ == 126){
        uint16_t length = 0;
        memcpy(&length, msg + pos, 2);
        pos += 2;
        payload_length_ = ntohs(length);
    }
    else if(payload_length_ == 127){
        uint32_t length = 0;
        memcpy(&length, msg + pos, 8);
        pos += 8;
        payload_length_ = ntohl(length);
    }
    //Masking-key
    if(mask_ == 1){
        for(int i = 0; i < 4; i++){
            masking_key_[i] = msg[pos + i];
        }
        pos += 4;
    }
    //取出消息数据
    if (msgLen >= pos + payload_length_ ){
        outBuf.clear();
        if(mask_ != 1){
            const char* dataBegin = msg + pos;
            outBuf.insert(outBuf.begin(), dataBegin, dataBegin+payload_length_);
        }
        else{
            for(uint i = 0; i < payload_length_; i++){
                int j = i % 4;
                outBuf.push_back(msg[pos + i] ^ masking_key_[j]);
            }
        }
    }
    else return -2; //此时包长小于报文中记录的包长
    
    //断开连接类型数据包
    if ((int)opcode_ == 0x8)
        return -1;
 
    return 0;
}


std::string  packData(const std::string& message){
   std::string data="";
    unsigned long n=message.size();
    if (n < 126){	 
        data.push_back(0x81);
        data.push_back(n);
        data+=message;
    }
    else if (n < 0xFFFF){
        data.push_back(0x81);
        data.push_back(126);
        data.push_back((n>>8 & 0xFF));
        data.push_back((n & 0xFF));
        data+=message; 
    }

    return data;
}


char dec2hexChar(short int n) {      
    if ( 0 <= n && n <= 9 ) {      
        return char( short('0') + n );      
    } else if ( 10 <= n && n <= 15 ) {      
        return char( short('A') + n - 10 );      
    } else {      
        return char(0);      
    }      
}      
  
short int hexChar2dec(char c) {      
    if ( '0'<=c && c<='9' ) {      
        return short(c-'0');      
    } else if ( 'a'<=c && c<='f' ) {      
        return ( short(c-'a') + 10 );      
    } else if ( 'A'<=c && c<='F' ) {      
        return ( short(c-'A') + 10 );      
    } else {      
        return -1;      
    }      
}      

std::string encodeURL(const std::string& URL)      {      
    std::string result = "";      
    for ( unsigned int i=0; i<URL.size(); i++ ) {      
        char c = URL[i];      
        if (      
            ( '0'<=c && c<='9' ) ||      
            ( 'a'<=c && c<='z' ) ||      
            ( 'A'<=c && c<='Z' ) ||      
            c=='/' || c=='.'      
            ) {      
                result += c;      
        } else {      
            int j = (short int)c;      
            if ( j < 0 ) {      
                j += 256;      
            }      
            int i1, i0;      
            i1 = j / 16;      
            i0 = j - i1*16;      
            result += '%';      
            result += dec2hexChar(i1);      
            result += dec2hexChar(i0);      
        }      
    }      
    return result;      
}      
  
std::string decodeURL(const std::string& URL) {      
    std::string result = "";      
    for ( unsigned int i=0; i < URL.size(); i++ ) {      
        char c = URL[i];      
        if ( c != '%' ) {      
            result += c;      
        } else {      
            char c1 = URL[++i];      
            char c0 = URL[++i];      
            int num = 0;      
            num += hexChar2dec(c1) * 16 + hexChar2dec(c0);      
            result += char(num);      
        }      
    }      
    return result;      
}    

}
