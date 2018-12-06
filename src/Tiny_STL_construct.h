#ifndef TINY_STL_CONSTRUCT_H
#define TINY_STL_CONSTRUCT_H

#include "Type_traits.h"
#include "Tiny_STL_iterator.h"
namespace Tiny_STL
{

/**placement new
* placement new允许我们将object创建于已经申请好的内存中,但是没有所谓的 
* placenment delete,因为根本没有分配内存,所以没有placement delete
* 
* char* buf = new char[sizeof(A) * 3];//申请了3个A的内存
* A* pc = new(buf)A();//运用申请好的buf的内存,在buf上赋值
* 
* 上面的new(buf)A();就是placement new.
* 
* 编译器遇到上面代码会翻译成
* A * pc;
* try {
*     void* men = operator new(sizeof(A), buf); //借用内存
*     pc = static_cast<A*>(mem);//安全转换
*     pc->A::A();//构造函数
* }
* catch (std::bad_alloc){
*     //若失败 不执行构造函数
* }
* 侯捷《c++内存管理》
*/
template<class T1, class T2>
inline void construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template<class T>
inline void destroy(T* pointer)
{
    pointer->~T();
}


template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for( ; first < last; ++first)
    {
        destroy(&*first);
    }
}

template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

template<class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

}
#endif