#include <iostream>

#include "../src/Tiny_STL_rbtree.h"
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
    __rb_tree_iterator<int, int&, int*>::link_type p;
    __rb_tree_iterator<int, int&, int*> iter(p);
    less<int> k;
    rb_tree<int, int, identity<int>, less<int>> rb;
    cout << rb.size() << endl;
    
    rb.insert_unique(10);
    rb.insert_unique(7);
    rb.insert_unique(8);
    rb.insert_unique(16);
    rb.insert_unique(2);
    rb.insert_unique(6);
    rb.insert_unique(4);
    rb.insert_unique(9);
    rb.insert_unique(13);
    cout << rb.size() << endl;
    for (auto iter = rb.begin();iter != rb.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    rb.erase(++rb.begin());
    rb.erase(--rb.end());
    auto iter1 = rb.find(9);
    rb.erase(iter1);
    for (auto iter = rb.begin();iter != rb.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    
    //移动赋值测试
    cout << "移动赋值测试" << endl;
    rb_tree<int, int, identity<int>, less<int>> rb2(rb);
    print(rb);
    print(rb2);
    rb_tree<int, int, identity<int>, less<int>> rb3(std::move(rb));
    print(rb);
    print(rb3);

    //初始化列表
    cout << endl << "初始化列表" << endl;
    rb_tree<int, int, identity<int>, less<int>> rb4{1, 2, 5, 7, 9, 3};
    print(rb4);
    cout << rb4.size() << endl;
}