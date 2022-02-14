#pragma once
#include <string>
#include <random>
#include <ctime>

class newrand{

public:
    newrand()=default;

public:
    unsigned nextUnsigned(unsigned min,unsigned max){
        std::uniform_int_distribution<unsigned> uid(min,max);//随机数分布类
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        return uid(dre);
    }

    int nextInt(int min,int max){//左闭右闭区间
        std::uniform_int_distribution<int> uid(min,max);//随机数分布类
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        return uid(dre);
    }

    std::string nextUpper(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid('A','Z');//随机数分布类
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i)result.at(i)=uid(dre);
        return result;
    }   

    std::string nextLower(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid('a','z');//随机数分布类 
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i)result.at(i)=uid(dre);
        return result;
    }

    std::string nextLetter(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid(0,25);//随机数分布类 
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i)result.at(i)=uid(dre)%2?uid(dre)+'a':uid(dre)+'A';
        return result;
    }

    std::string nextDigitAndUpper(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid(0,25);//随机数分布类 
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i)result.at(i)=uid(dre)%2?uid(dre)+'0':uid(dre)+'A';
        return result;
    }


    std::string nextDigitAndLower(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid(0,25);//随机数分布类 
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i)result.at(i)=uid(dre)%2?uid(dre)+'0':uid(dre)+'a';
        return result;
    }

    std::string nextDigitAndLetter(int length){
        std::string result(length,' ');
        std::uniform_int_distribution<int> uid(0,25);//随机数分布类 
        static std :: default_random_engine dre(time(0)); //随机数引擎类
        for(int i=0;i<length;++i){
            if(uid(dre)%3==0)result.at(i)=uid(dre)%10+'0';
            else if(uid(dre)%3==1)result.at(i)=uid(dre)+'a';
            else result.at(i)=uid(dre)+'A';
        }
        return result;
    }

};