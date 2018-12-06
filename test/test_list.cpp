#include <iostream>

#include "../src/Tiny_STL_list.h"
using namespace Tiny_STL;
using std::cout;
using std::endl;

int main()
{
    list<int> list1;
    list1.push_back(1);
    list1.push_back(4);
    list1.push_back(6);
    list1.push_back(10);
    list1.push_back(2);
    list1.sort();
    for(auto iter=list1.begin();iter!=list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    list<int> list2;
    list2.push_back(8);
    list2.push_back(5);
    list2.push_back(3);
    list2.push_back(9);
    list2.push_back(7);
    list2.sort();
    list1.merge(list2);
    for(auto iter=list1.begin();iter!=list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    cout << list1.size() << endl;
    cout << list1.empty() << endl;
    cout << list1.front() << endl;
    cout << list1.back() << endl;
    list1.insert(list1.begin(), 3);
    list1.push_back(4);
    list1.push_front(5);
    for(auto iter=list1.begin();iter!=list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    list1.pop_back();
    list1.pop_front();
    list1.remove(6);
    for(auto iter=list1.begin();iter!=list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
    list1.reverse();
    for(auto iter=list1.begin();iter!=list1.end();++iter)
        cout << *iter << "  ";
    cout << endl;
}