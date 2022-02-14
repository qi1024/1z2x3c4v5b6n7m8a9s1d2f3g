/*#include <iostream>
#include "../thread_pool.hpp"
#include <string>

class Test{
public:
    Test(std::string name):pname(name){}

    void process(){
        for(int i=0;i<1000;++i){
            std::cout<<"调用了"<<pname<<"的函数"<<std::endl;
        }
    }
private:
    std::string pname;
};

int main(){
    thread_pool<Test>::getInstance(8,8)->append(new Test("张三"));
    thread_pool<Test>::getInstance(8,8)->append(new Test("刘备"));
    thread_pool<Test>::getInstance(8,8)->append(new Test("关羽"));
    //std::cout<<"运行成功"<<std::endl;
    std::cin.get();
    return 0;
}
*/