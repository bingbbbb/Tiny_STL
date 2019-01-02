#include <iostream>
#include <vector>
#include "../src/test.cpp"
using namespace std;
class A
{
    public:
    A(){cout << "A()" << endl;}
    ~A(){cout << "~A()" << endl;}
};
class B
{
    public:
    A a;
    B(){cout << "B()" << endl;}
    ~B(){cout << "~B()" << endl;}
};

int main()
{
    vector<int> vec{1, 2, 3};
}