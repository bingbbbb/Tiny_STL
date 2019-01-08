#include <iostream>
#include <thread>    
#include <unistd.h>     //usleep

#include "../src/Tiny_STL_queue.h"
using namespace Tiny_STL;
using std::cout;
using std::endl;

class test
{
    public:
    test() : data(0){cout << "test() " << data << endl;}
    test(int i) : data(i){cout << "test(int i) " << data << endl;}
    test(const test& other) : data(other.data){cout << "test(const test& other) " << data << endl;}
    ~test(){cout << "~test() " << data << endl;} 
    int data;
};
void fun1(queue<int>& q)
{
    for (int i = 0;i < 20;++i)
    {
        if (i >= 10)
            usleep(1000);
        q.push(i);
        printf("push() : %d \n", i);
    }
}
void fun2(queue<int>& q)
{
    for (int i = 0;i < 10;++i)
    {
        auto p = q.try_pop();
        if (p.get() != nullptr)
            printf("try_pop() : %d \n", *p);
        else
            printf("try_pop() :  \n");
    }
}
void fun3(queue<int>& q)
{
    for (int i = 0;i < 10;++i)
    {
        
        auto p = q.wait_and_pop();
        printf("wait_and_pop() : %d \n", *p);
    }
}
int main()
{
    queue<test> q;
    for (int i = 0;i < 5;++i)
        q.push(i);
    while(!q.empty())
    {
        auto p = q.try_pop();
        //auto p = q.wait_and_pop();
        cout << (*p).data << endl;
    }
    auto p = q.try_pop();
    bool isNull = (p.get() == nullptr);
    cout << "p is nullptr ? :" << isNull << endl;

    queue<int> q_int;
    std::thread t1(std::bind(fun1, std::ref(q_int)));
    std::thread t2(std::bind(fun2, std::ref(q_int)));
    std::thread t3(std::bind(fun3, std::ref(q_int)));
    t1.join();
    t2.join();
    t3.join();
}