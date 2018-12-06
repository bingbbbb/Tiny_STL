#include <iostream>

#include "../src/Tiny_STL_vector.h"

using namespace Tiny_STL;
using std::cout;
using std::endl;

int main()
{
    vector<int> vec(2,9);
    cout << "size: " << vec.size() << endl;
    cout << "capacity: " << vec.capacity() << endl;
    vec.push_back(1);
    cout << "size: " << vec.size() << endl;
    cout << "capacity: " << vec.capacity() << endl;
    vec.push_back(2);
    cout << "size: " << vec.size() << endl;
    cout << "capacity: " << vec.capacity() << endl;
    vec.push_back(3);
    cout << "size: " << vec.size() << endl;
    cout << "capacity: " << vec.capacity() << endl;
    vec.push_back(4);
    cout << "size: " << vec.size() << endl;
    cout << "capacity: " << vec.capacity() << endl;
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
    auto iter1 = vec.begin();
    auto iter2 = vec.end();
    --iter2;++iter1;
    vec.erase(iter1,iter2);
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
    vec.insert(iter1, 5, 1);
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
    cout << vec.front() << endl;
    cout << vec.back() << endl;
    vec.pop_back();
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
    vec.clear();
    cout << vec.empty() << endl;
    vec.resize(5,4);
    for(int i=0;i<vec.size();i++)
        cout << vec[i] << "  ";
    cout << endl;
}