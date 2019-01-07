#ifndef TINY_STL_VECTOR_H
#define TINY_STL_VECTOR_H

#include <initializer_list>
#include <mutex>
#include <condition_variable>

#include "Tiny_STL_alloc.h"
#include "Tiny_STL_construct.h"
#include "Tiny_STL_uninitialized.h"
//#include "Tiny_STL_iterator.h"

namespace Tiny_STL
{
template<class T, class Alloc = __default_alloc>
class vector
{
//型别定义
typedef T           value_type;
typedef T*          pointer;
typedef T&          reference;
typedef const T&    const_reference;
typedef size_t      size_type;
typedef ptrdiff_t   difference_type;
//定义迭代器
typedef T*          iterator;
//定义空间配置器
typedef simple_alloc<value_type, Alloc> data_allocator;

public:
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {};
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    vector(int n, const T& value) { fill_initialize(n, value); }
    vector(long n, const T& value) { fill_initialize(n, value); }
//阻止隐式装换，当用户误用（如vector<int> vec;vec = 3;）时，编译不通过
    explicit vector(size_type n) { fill_initialize(n, T()); }
    vector(const vector<T, Alloc>& x) 
    {
        start = data_allocator::allocate(x.end() - x.begin());
        uninitialized_copy(x.begin(), x.end(), start);
        finish = start + (x.end() - x.begin());
        end_of_storage = finish;
    }
    vector(vector<T, Alloc>&& x) : start(x.start), finish(x.finish), end_of_storage(x.end_of_storage)
    {
        x.start = nullptr;
        x.finish = nullptr;
        x.end_of_storage = nullptr;
    }
    vector(std::initializer_list<T> iniList) : start(nullptr), finish(nullptr), end_of_storage(nullptr) 
    {
        auto size = iniList.size();
        start = data_allocator::allocate(size);
        finish = start;
        for (auto iter = iniList.begin();iter != iniList.end();++iter)
        {
            construct(finish, *iter);
            ++finish;
        }
        end_of_storage = finish;
    }
    ~vector()
    {
        destroy(start, finish);
        deallocate();
    }
    reference front() { return *start; }
    reference back() { return *(finish-1); }

    void push_back(const T& x)
    {
        if (finish != end_of_storage)
        {
            construct(finish, x);
            ++finish;
        }
        else
            insert_aux(end(), x);
    }
    void pop_back()
    {
        --finish;
        destroy(finish);
    }
    iterator erase(iterator position)
    {
        iterator iter = position;
        while(iter != finish - 2)
        {
            *iter = *(iter + 1);
            ++iter;
        }
        --finish;
        destroy(finish);
        return position;
    }
    void resize(size_type new_size, const T& x);
    void resize(size_type new_size) { return resize(new_size, T()); }
    void clear() { erase(start, finish); }
    iterator erase(iterator first, iterator last)
    {//erase [first, last)
        iterator iter = first;
        size_type n = last - first;
        while(iter != finish - n)
        {
            *iter = *(iter + n);
            ++iter;
        }
        while(n > 0)
        {
            --finish;
            destroy(finish);
            --n;
        }
        return first;
    }
    void insert(iterator position, size_type n, const T& x);

    iterator begin() const { return start; }
    iterator end() const { return finish; }
    size_type size() const { return finish - start; }
    size_type capacity() const { return end_of_storage - start; }
    bool empty() const { return start == finish; }
    reference operator[](size_type n) { return *(start + n); }
    const_reference operator[](size_type n) const { return *(begin() + n); }

    //vector(const vector<T, Alloc> &) = delete;
    vector<T, Alloc>&operator=(const vector<T, Alloc>&) = delete;

private:
    void insert_aux(iterator position, const T& value);
    void deallocate();
    void fill_initialize(size_type n, const T& value);
    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }

    iterator start;
    iterator finish;
    iterator end_of_storage;
    std::mutex m;
    std::condition_variable cond;
};

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& value)
{
    //从未使用过的空间需要使用construct构造(Tiny_STL_construct.h 内部函数
    //都是使用construct构造)，其他可以直接使用拷贝赋值=
    if (finish != end_of_storage)
    {//还有备用空间
        construct(finish,*(finish - 1));
		++finish;
        for(iterator iter=finish - 2;iter != position;iter--)
            *iter = *(iter - 1);
        *position = value;
    }
    else
    {
        const size_type old_size = size();
        const size_type len = old_size != 0 ? 2 * old_size : 1;

        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        new_finish = uninitialized_copy(start, position, new_start);
        construct(new_finish, value);
        ++new_finish;
        new_finish = uninitialized_copy(position, finish, new_finish);

        destroy(start, finish);
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

template<class T, class Alloc>
void vector<T, Alloc>::deallocate()
{
    if (start)
        data_allocator::deallocate(start, end_of_storage - start);
}

template<class T, class Alloc>
void vector<T, Alloc>::fill_initialize(size_type n, const T& value)
{//该函数含有alloc功能
    start = allocate_and_fill(n, value);
    finish = start + n;
    end_of_storage = finish;
}

template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size, const T& x)
{
    if (new_size < size())
        erase(start + new_size, finish);
    else 
        insert(finish, new_size - size(), x);
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
    if (n == 0) return;
    if (size_type(end_of_storage - finish) > n)
    {
        const size_type elems_after = finish - position;
        iterator old_finish = finish;
        if (elems_after > n)
        {
            uninitialized_copy(finish - n, finish, finish);
            finish += n;
            iterator iter = old_finish - 1;
            while(iter - position > n - 1)
            {
                *iter = *(iter - n);
                --iter;
            }
            while(iter != position - 1 )
            {
                *iter = x;
                --iter;
            }
        }
        else
        {
            uninitialized_fill_n(finish, n - elems_after, x);
            finish += n - elems_after;
            uninitialized_copy(position, old_finish, finish);
            finish += elems_after;
            iterator iter = position;
            while(iter != old_finish)
            {
                *iter = x;
                ++iter;
            }
        }
    }
    else
    {
        const size_type old_size = size();
        const size_type len = old_size * 2 + n;

        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        new_finish = uninitialized_copy(start, position, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, x);
        new_finish = uninitialized_copy(position, finish, new_finish);

        destroy(start, finish);
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

}
#endif