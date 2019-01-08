#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

namespace Tiny_STL
{
//前向声明
template<class T> class my_weak_ptr;

//my_shared_ptr和my_weak_ptr的控制块类
class count
{
public:
    count() : shared_count(0), weak_count(0) {}
    count(long s_c, long w_c) : shared_count(s_c), weak_count(w_c) {}
    long shared_count;
    long weak_count;
};

template<class T>
class my_shared_ptr
{
public:
    my_shared_ptr() : ptr(nullptr), count_ptr(nullptr) {}
    explicit my_shared_ptr(T* p) : ptr(p), count_ptr(new count(1, 0)) {}
    my_shared_ptr(const my_shared_ptr<T>& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    { 
        ++(count_ptr->shared_count); 
    }
    my_shared_ptr(my_shared_ptr<T>&& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    { 
        other.ptr = nullptr;
        other.count_ptr = nullptr; 
    }
    my_shared_ptr(const my_weak_ptr<T>& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    { 
        ++(count_ptr->shared_count); 
    }
    ~my_shared_ptr()
    {
        if (ptr != nullptr && count_ptr != nullptr)
        {
            --(count_ptr->shared_count);
            if (count_ptr->shared_count == 0)
            {
                delete ptr;
                if (count_ptr->weak_count == 0)
                    delete count_ptr;
            }
        }
    }
    void reset() { my_shared_ptr().swap(*this); }
    void reset(T* p) { my_shared_ptr<T>(p).swap(*this); }
    void swap(my_shared_ptr<T>& other)
    {
        T* tmp_1 = ptr;
        ptr = other.ptr;
        other.ptr = tmp_1;
        count* tmp_2 = count_ptr;
        count_ptr = other.count_ptr;
        other.count_ptr = tmp_2;
    }
    //重载=，调用复制构造函数产生临时值，然后与this交换，生命周期到达时
    //临时值自动调用析构函数
    my_shared_ptr<T>& operator=(const my_shared_ptr<T>& other)
    {
        my_shared_ptr<T>(other).swap(*this);
        return *this;
    }
    my_shared_ptr<T>& operator=(my_shared_ptr<T>&& other)
    {
        my_shared_ptr<T>(std::move(other)).swap(*this);
        return *this;
    }
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    T* get() { return ptr; }
    long use_count() { return count_ptr ? count_ptr->shared_count : 0; }

    friend class my_weak_ptr<T>;

private:
    T* ptr;
    count* count_ptr;
};

template<class T, class... Args>
my_shared_ptr<T> make_shared(Args&&... args)
{
    return my_shared_ptr<T>(new T(std::forward<Args>(args)...));
}


template<class T>
class my_weak_ptr
{
public:
    my_weak_ptr() : ptr(nullptr), count_ptr(nullptr) {}
    my_weak_ptr(const my_weak_ptr<T>& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    {
        ++(count_ptr->weak_count);
    }
    my_weak_ptr(const my_shared_ptr<T>& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    {
        ++(count_ptr->weak_count);
    }
    my_weak_ptr(my_weak_ptr<T>&& other) : ptr(other.ptr), count_ptr(other.count_ptr)
    {
        other.ptr = nullptr;
        other.count_ptr = nullptr;
    }
    ~my_weak_ptr()
    {
        if (ptr != nullptr && count_ptr != nullptr)
        {
            --(count_ptr->weak_count);
            if (count_ptr->weak_count == 0 && count_ptr->shared_count == 0)
            {
                delete count_ptr;
            }
        }
    }
    my_weak_ptr<T>& operator=(const my_weak_ptr<T>& other)
    {
        my_weak_ptr<T>(other).swap(*this);
        return *this;
    }
    my_weak_ptr<T>& operator=(my_weak_ptr<T>&& other)
    {
        my_weak_ptr<T>(std::move(other)).swap(*this);
        return *this;
    }
    my_weak_ptr<T>& operator=(const my_shared_ptr<T>& other)
    {
        my_weak_ptr<T>(other).swap(*this);
        return *this;
    }
    void reset()
    {
        ptr = nullptr;
        count_ptr = nullptr;
    }
    void swap(my_weak_ptr<T>& other)
    {
        T* tmp_1 = ptr;
        ptr = other.ptr;
        other.ptr = tmp_1;
        count* tmp_2 = count_ptr;
        count_ptr = other.count_ptr;
        other.count_ptr = tmp_2;
    }
    long use_count() { return count_ptr ? count_ptr->shared_count : 0; }
    bool expired() { return use_count() == 0; }
    my_shared_ptr<T> lock() { return expired() ? my_shared_ptr<T>() : my_shared_ptr<T>(*this); }
    
    friend class my_shared_ptr<T>;

private:
    T* ptr;
    count* count_ptr;
};

}
#endif