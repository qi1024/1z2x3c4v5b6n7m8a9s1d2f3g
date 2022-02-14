#include <iostream>
#include "../log.hpp"



int main(){

    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"这是一条测试日志信息4");
    for(int i=0;i<1000;++i)for(int j=0;j<1000;++j);
    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"这是一条测试日志信息5");
    for(int i=0;i<1000;++i)for(int j=0;j<1000;++j);
    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"这是一条测试日志信息6");
    for(int i=0;i<1000;++i)for(int j=0;j<1000;++j);
    std::cin.get();
    return 0;
}