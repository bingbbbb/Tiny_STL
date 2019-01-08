#ifndef SMART_PTR_H
#define SMART_PTR_H
//#include <iostream>
//using std::cout;
//using std::endl;
namespace Tiny_STL
{
template<class T>
class my_unique_ptr
{
public:
    my_unique_ptr() : ptr(nullptr) {}
    ~my_unique_ptr() 
    { 
        if (ptr != nullptr)
            delete ptr; 
    }
    explicit my_unique_ptr(T* p) : ptr(p) {}
    my_unique_ptr(my_unique_ptr<T>&& other) : ptr(other.ptr)
    {
        //cout << " my_unique_ptr(my_unique_ptr<T>&& other)" << endl;
        other.ptr = nullptr;
    }
    my_unique_ptr<T>& operator=(my_unique_ptr<T>&& other)
    {
        if (ptr == other.ptr)
            return *this;
        T* old_ptr = ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        if (old_ptr != nullptr)
            delete old_ptr;
    }
    T* get() { return ptr; }
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    void reset(T* p)
    {
        T* old_ptr = ptr;
        ptr = p;
        if (old_ptr != nullptr)
            delete old_ptr;
    }
    T* release()
    {
        T* old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }
    void swap(my_unique_ptr<T>& other)
    {
        T* tmp = ptr;
        ptr = other.ptr;
        other.ptr = tmp;
    }

    my_unique_ptr(const my_unique_ptr<T>& other) = delete;
    my_unique_ptr<T>& operator=(const my_unique_ptr<T>& other) = delete;
private:
    T* ptr;
};

template<class T> 
class my_unique_ptr<T[]>
{
public:
    my_unique_ptr() : ptr(nullptr) {}
    ~my_unique_ptr() 
    { 
        if (ptr != nullptr)
            delete[] ptr; 
    }
    explicit my_unique_ptr(T* p) : ptr(p) {}
    my_unique_ptr(my_unique_ptr<T[]>&& other) : ptr(other.ptr)
    {
        //cout << " my_unique_ptr(my_unique_ptr<T>&& other)" << endl;
        other.ptr = nullptr;
    }
    my_unique_ptr<T[]>& operator=(my_unique_ptr<T[]>&& other)
    {
        if (ptr == other.ptr)
            return *this;
        T* old_ptr = ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        if (old_ptr != nullptr)
            delete[] old_ptr;
    }
    T* get() { return ptr; }
    T& operator[](size_t i) { return ptr[i]; }
    void reset(T* p)
    {
        T* old_ptr = ptr;
        ptr = p;
        if (old_ptr != nullptr)
            delete[] old_ptr;
    }
    T* release()
    {
        T* old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }
    void swap(my_unique_ptr<T[]>& other)
    {
        T* tmp = ptr;
        ptr = other.ptr;
        other.ptr = tmp;
    }

    my_unique_ptr(const my_unique_ptr<T[]>& other) = delete;
    my_unique_ptr<T>& operator=(const my_unique_ptr<T[]>& other) = delete;
private:
    T* ptr;
};

//只能用于非数组
/**
template<class T, class... Args>
my_unique_ptr<T> make_unique(Args&&... args)
{
    return my_unique_ptr<T>(new T(std::forward<Args>(args)...));
}
*/

//remove_extent应该放在Type_traits.h，放到这里只是方便阅读
template<class T>
struct remove_extent
{ typedef T     type; };

template<class T, std::size_t _Size>
struct remove_extent<T[_Size]>
{ typedef T     type; };

template<class T>
struct remove_extent<T[]>
{ typedef T     type; };

template<class T>
using remove_extent_t = typename remove_extent<T>::type;


template<class T>
struct _MakeUniq
{ typedef my_unique_ptr<T> __single_object; };

template<class T>
struct _MakeUniq<T[]>
{ typedef my_unique_ptr<T[]> __array; };

template<class T, size_t _Bound>
struct _MakeUniq<T[_Bound]>
{ struct __invalid_type { }; };

/// std::make_unique for single objects
template<class T, class... _Args>
inline typename _MakeUniq<T>::__single_object
make_unique(_Args&&... __args)
{ return my_unique_ptr<T>(new T(std::forward<_Args>(__args)...)); }

/// std::make_unique for arrays of unknown bound
template<class T>
inline typename _MakeUniq<T>::__array
make_unique(size_t __num)
{ return my_unique_ptr<T>(new remove_extent_t<T>[__num]()); }

/// Disable std::make_unique for arrays of known bound
template<class T, class... _Args>
inline typename _MakeUniq<T>::__invalid_type
make_unique(_Args&&...) = delete;

}
#endif