#include <iostream>
using namespace std;
class test
{
public:
    int i;
    test(){ cout << "test construct" << endl; }
    test(int n){ cout << "test(size_t n) construct" << endl; i=n; }
    ~test(){ cout << "test destruct" << endl; }
    void print(){ cout << "i : " << i << endl; }
};