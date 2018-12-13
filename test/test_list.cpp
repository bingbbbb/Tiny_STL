#include <iostream>

#include "../src/Tiny_STL_list.h"
using namespace Tiny_STL;
using std::cout;
using std::endl;

template<class T>
void print(list<T> list1)
{
    for(auto iter = list1.begin();iter != list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
}

int main()
{
    //基本功能测试
    list<int> list1;
    list1.push_back(1);
    list1.push_back(4);
    list1.push_back(6);
    list1.push_back(10);
    list1.push_back(2);
    list1.sort();
    print(list1);
    list<int> list2;
    list2.push_back(8);
    list2.push_back(5);
    list2.push_back(3);
    list2.push_back(9);
    list2.push_back(7);
    list2.sort();
    list1.merge(list2);
    print(list1);
    cout << list1.size() << endl;
    cout << list1.empty() << endl;
    cout << list1.front() << endl;
    cout << list1.back() << endl;
    list1.insert(list1.begin(), 3);
    list1.push_back(4);
    list1.push_front(5);
    print(list1);
    list1.pop_back();
    list1.pop_front();
    list1.remove(6);
    print(list1);
    list1.reverse();
    print(list1);

    //移动赋值测试
    cout << "移动赋值测试" << endl;
    list<int> list3(list1);
    print(list1);
    print(list3);
    list<int> list4(std::move(list1));
    print(list1);
    print(list4);

    //初始化列表
    cout << endl << "初始化列表" << endl;
    list<int> list5{1, 2, 5, 7, 9, 3};
    print(list5);

}