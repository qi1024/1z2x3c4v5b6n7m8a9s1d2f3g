
/*实现一个半同步/半反应堆并发模式的线程池
原理：主线程往工作队列中插入任务，工作线程通过竞争来取得任务并执行它
前提：所有的客户请求都是无状态的，因为同一个连接上的不同请求可能会由不同的请求处理
*/

#pragma once
#include <list>
#include <cassert>
#include <pthread.h>
#include "../tools/locker.hpp"
#include "../tools/log.hpp"

/*线程池类，模板参数是任务类*/
template<typename T>
class thread_pool{
private:
    /*参数thread_number是线程池中线程的数量
    max_reque是请求队列中最多允许的、等待处理的请求数量
    */
    thread_pool();
    static void* work(void* arg);

public:
    static thread_pool* getInstance();
    ~thread_pool();
    bool append(T request);//往请求队列中添加任务

private:
    static thread_pool* pool;
    int thread_number=16;                  /*线程池中的线程数*/
    int max_requests=1024;                   /*请求队列中允许的最大请求数*/
    pthread_t* threads=nullptr;                 /*线程描述符数组，大小也是thread_number*/
    std::list<T>work_queue;            /*请求队列*/
    locker queue_locker;                /*保护请求队列的互斥锁*/
   // sem queuestat;                      /*是否有任务需要处理*/
    bool pstop;                         /*是否结束线程*/
   static pthread_mutex_t mutex;/*互斥锁*/
};

/*静态成员初始化*/

template<typename T>
thread_pool<T>* thread_pool<T>::pool=nullptr;

template<typename T>
pthread_mutex_t thread_pool<T>::mutex=PTHREAD_MUTEX_INITIALIZER;


template<typename T>
thread_pool<T>* thread_pool<T>::getInstance(){
     if(pool==nullptr){
        pthread_mutex_lock(&mutex);
        if(pool==nullptr){
            pool=new thread_pool();
            assert(pool);
        }
        pthread_mutex_unlock(&mutex);
    }
    return pool;
}

template<typename T>
thread_pool<T>::thread_pool(){
    assert(thread_number>=0 && max_requests>=0);
    threads=new pthread_t[thread_number];
    assert(threads);
    /*创建thread_number个线程，并将它们设置为脱离线程*/
    for(int i=0;i<thread_number;++i){
        LOG::getInstance()->log(__FILE__,__func__,__LINE__,"创建第"+std::to_string(i+1)+"个线程");
        if(pthread_create(threads+i,nullptr,work,nullptr)!=0){
            delete[] threads;   //只要有一个线程创建失败，之前创建好的都要释放
            assert(false);
        }
        if(pthread_detach(threads[i])){
            delete[] threads;   //只要有一个线程与主线程分离失败，之前创建好的都要释放
            assert(false);
        }
    }
}



template<typename T>
thread_pool<T>::~thread_pool(){
    delete[] threads;
    pstop=true;
    LOG::getInstance()->log(__FILE__,__func__,__LINE__,"线程池销毁完成");
}

template<typename T>
bool thread_pool<T>::append(T request){
    pthread_mutex_lock(&mutex);
    if(work_queue.size()>max_requests){
        pthread_mutex_unlock(&mutex);
        return false;
    }
    work_queue.push_back(request);
    pthread_mutex_unlock(&mutex);
    return true;
}

template<typename T>
void* thread_pool<T>::work(void* arg){
    while(!getInstance()->pstop){
        pthread_mutex_lock(&mutex);
        if(getInstance()->work_queue.empty()){
            pthread_mutex_unlock(&mutex);
            continue;
        }
        T* request=&(getInstance()->work_queue.front());
        getInstance()->work_queue.pop_front();
        pthread_mutex_unlock(&mutex);
        if(!request)continue;
        else request->process();
    }
    return pool;
}







