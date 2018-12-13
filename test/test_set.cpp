#include <iostream>

#include "../src/Tiny_STL_set.h"
using namespace Tiny_STL;
using std::cout;
using std::endl;
template<class T>
void print(T temp)
{
    for(auto iter = temp.begin();iter != temp.end();++iter)
        cout << *iter << "  ";
    cout << endl;
}

int main()
{
    //基本功能测试
    set<int> set_;
    set_.insert(3);
    set_.insert(9);
    set_.insert(2);
    set_.insert(5);
    set_.insert(6);
    set_.insert(8);
    set_.insert(1);
    set_.insert(3);
    cout << "size(): " << set_.size() << endl;
    for(auto iter = set_.begin();iter != set_.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    auto iter1 = set_.find(5);
    set_.erase(iter1);
    set_.erase(++set_.begin());
    for(auto iter = set_.begin();iter != set_.end();++iter)
        cout << *iter << "  ";
    cout << endl;

    //移动赋值测试
    cout << "移动赋值测试" << endl;
    set<int> set2(set_);
    print(set_);
    print(set2);
    set<int> set3(std::move(set_));
    print(set_);
    print(set3);

    //初始化列表
    cout << endl << "初始化列表" << endl;
    set<int> set4{1, 2, 5, 7, 9, 3};
    print(set4);
}