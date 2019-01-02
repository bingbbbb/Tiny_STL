# Tiny_STL
* 编译
>   git clone https://github.com/bingbbbb/Tiny_STL.git  
>   cd Tiny_STL  
>   bash ./build.sh  
* 运行测试代码
>   ./bin/test_vector  
>   ./bin/test_list  
>   ./bin/test_rbtree  
>   ./bin/test_set  
***
***
## 目前实现的组件
* 空间配置器
* vector
* list
* rbtree
* set   
TODO vector实现stack
红黑树比较难，有些函数是直接抄的，现在还没看懂
***
***
## 基本功能
基本的构造析构，size，begin，end，erase， insert及其他常用函数
***
***
## c++11特性
参考《STL源码剖析》实现的基本功能，加入常用的c++11特性。c++ primer plus
有提到c++11的STL新加入的功能，c++ concurrency in action提及如何构造线程
安全的数据结构。
***
### 移动语义
利用移动语义和右值引用，移动构造函数可避免数据拷贝。
***
### 初始化列表
一种初始化方法(如 vector<int> vec{1, 2, 5, 7, 9, 3};)  
***
### 线程安全函数
TODO：线程安全stack
