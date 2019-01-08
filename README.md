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
>   ./bin/test_queue  
>   ./bin/test_ptr  
***
***
## 目前实现的组件
* 空间配置器
* vector
* list
* rbtree
* set   
* queue  
***
***
## 基本功能
基本的构造析构，size，begin，end，erase， insert及其他常用函数,每个功能都进行了测试，见test文件夹
***
***
## c++11特性
参考《STL源码剖析》实现的基本功能，加入常用的c++11特性。c++ primer plus
有提到c++11的STL新加入的功能，c++ concurrency in action提及如何构造线程
安全的数据结构。
***
### 移动语义
利用移动语义和右值引用，移动构造函数可避免数据拷贝。 
>  vector<int> vec1(2,9);  
>  vector<int> vec2(std::move(vec1));  
***
### 初始化列表
一种初始化方法  
>  vector<int> vec{1, 2, 5, 7, 9, 3};  
***
### 智能指针   
实现my_unique_ptr，my_weak_ptr，my_shared_ptr，功能基本与std::unique_ptr, std::weak_ptr, std::shared_ptr相同  
#### my_unique_ptr  
my_unique_ptr有两个版本：  
1) 管理单个对象  
2) 管理对象数组  

my_unique_ptr类满足可移动构造和可移动赋值的要求，但不满足可复制构造或可复制赋值的要求。两个版本都支持用make_unique构造，应避免直接使用new构造，否则会有
代码冗余和异常安全。(《effective modern c++》条款21) 

#### my_weak_ptr  
my_weak_ptr 是一种智能指针，对被 my_shared_ptr 管理的对象存在非拥有性（「弱」）引用。在访问所引用的对象前必须先转换为 my_shared_ptr。  

#### my_shared_ptr 
my_shared_ptr 是通过指针保持对象共享所有权的智能指针。多个 my_shared_ptr 对象可占有同一对象。下列情况之一出现时销毁对象并解分配其内存：
1) 最后剩下的占有对象的 my_shared_ptr 被销毁；  
2) 最后剩下的占有对象的 my_shared_ptr 被通过 operator= 或 reset() 赋值为另一指针。   

TODO： enable_shared_from_this
***
### 线程安全类
#### 线程安全queue  
提供 try_pop(), wait_and_pop(), push(), empty()等函数。  
底层使用智能指针 my_shared_ptr 和 my_unique_ptr 进行内存管理。  
