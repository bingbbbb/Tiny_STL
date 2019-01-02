#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>       
#include <functional>   //ref
#include <unistd.h>     //sleep()

#include "../src/Tiny_STL_vector.h"

using namespace Tiny_STL;
using std::cout;
using std::endl;

template<class T>
void print(vector<T> vec)
{
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
}

void fun_1(vector<int>& vec)
{
    for(int i=0;i<100000;i++)
        vec.push_back(i);
}

void fun_2(vector<int>& vec)
{
    for(int i=0;i<100000;i++)
        vec.push_back_safe(i);
}

void fun_3(vector<int>& vec)
{
    int x=0;
    for(int i=0;i<100;i++)
    {
        if(vec.empty_safe())
        {
            vec.wait_pop_back(x);
            printf("fun_3 wait_pop_back : %d \n", x);
        }
        else
        {
            vec.pop_back_safe(x);
            printf("fun_3 pop_back_safe : %d \n", x);
        }
    }
}

void fun_4(vector<int>& vec)
{
    int x=0;
    for(int i=0;i<100;i++)
    {
        if(vec.empty_safe())
        {
            vec.wait_pop_back(x);
            printf("fun_4 wait_pop_back : %d \n", x);
        }
        else
        {
            vec.pop_back_safe(x);
            printf("fun_4 pop_back_safe : %d \n", x);
        }
    }
}

void fun_5(vector<int>& vec)
{
    for(int i=0;i<200;i++)
    {
        vec.push_back_safe(i);
        printf("fun_5 push_back_safe : %d \n", i);
        usleep(1);
    }
        
}

int main()
{
    //基本功能测试
    vector<int> vec1(2,9);
    cout << "size: " << vec1.size() << endl;
    cout << "capacity: " << vec1.capacity() << endl;
    vec1.push_back(1);
    cout << "size: " << vec1.size() << endl;
    cout << "capacity: " << vec1.capacity() << endl;
    vec1.push_back(2);
    cout << "size: " << vec1.size() << endl;
    cout << "capacity: " << vec1.capacity() << endl;
    vec1.push_back(3);
    cout << "size: " << vec1.size() << endl;
    cout << "capacity: " << vec1.capacity() << endl;
    vec1.push_back(4);
    cout << "size: " << vec1.size() << endl;
    cout << "capacity: " << vec1.capacity() << endl;
    print(vec1);
    auto iter1 = vec1.begin();
    auto iter2 = vec1.end();
    --iter2;++iter1;
    vec1.erase(iter1,iter2);
    print(vec1);
    vec1.insert(iter1, 5, 1);
    print(vec1);
    cout << vec1.front() << endl;
    cout << vec1.back() << endl;
    vec1.pop_back();
    print(vec1);
    vec1.clear();
    cout << vec1.empty() << endl;
    vec1.resize(5, 4);
    print(vec1);

    //移动赋值测试
    cout << endl << "移动赋值测试" << endl;
    vector<int> vec2(vec1);
    print(vec1);
    print(vec2);
    vector<int> vec3(std::move(vec1));
    print(vec1);
    print(vec3);

    
    clock_t start,end;
    vec1.resize(1000000, 4);
    start = clock();
    vector<int> vec4(vec1);
    end = clock();
	double endtime = (double)(end-start)/CLOCKS_PER_SEC;
	cout << "复制构造函数调用时间:" << endtime * 1000 << "ms" << endl;
    start = clock();
    vector<int> vec5(std::move(vec1));
    end = clock();
	endtime = (double)(end-start)/CLOCKS_PER_SEC;
	cout << "移动构造函数调用时间:" << endtime * 1000 << "ms" << endl;

    //初始化列表
    cout << endl << "初始化列表" << endl;
    vector<int> vec6{1, 2, 5, 7, 9, 3};
    print(vec6);
    cout << "size: " << vec6.size() << "  capacity: " << vec6.capacity() << endl;


}