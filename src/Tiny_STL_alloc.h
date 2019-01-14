#ifndef TINY_STL_ALLOC_H
#define TINY_STL_ALLOC_H

#include <iostream> 
#include <stddef.h>     //size_t
#include <malloc.h>     //malloc()

#ifndef __THROW_BAD_ALLOC		
#define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1);
#endif
namespace Tiny_STL
{

/**
 * 容器直接使用simple_alloc类，该类的成员函数都是转调用第一级或第二级的配置器，
 * 由模板参数Alloc选择配置器。
 */
class __malloc_alloc;
class __default_alloc;

template<class T, class Alloc = __malloc_alloc>
class simple_alloc
{
public:
    static T *allocate(size_t n)
        { return 0 == n? 0 : static_cast<T*>(Alloc::allocate(n * sizeof(T))); }
    static T *allocate()
        { return static_cast<T*>(Alloc::allocate(sizeof(T))); }
    static void deallocate(T *p, size_t n)
        { if (0 != n) Alloc::deallocate(p, n * sizeof(T)); }
    static void deallocate(T *p)
        { Alloc::deallocate(p, sizeof(T)); }
};

/**
 * 第一级配置器
 */
class __malloc_alloc
{
private:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)();
public:
    static void * allocate(size_t n)
    {
        void *result = malloc(n);
        if (0 == result) result = oom_malloc(n);
        return result;
    }
    static void deallocate(void *p, size_t)
        { free(p); }
    static void * reallocate(void *p, size_t old_sz, size_t new_sz)
    {
        void *result = realloc(p, new_sz);
        if (0 == result) result = oom_realloc(p, new_sz);
        return result; 
    }
    static void (* set_malloc_handler(void (*f)()))()
    {
        void (* old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return(old);
    }
};

void (* __malloc_alloc::__malloc_alloc_oom_handler)() = 0;

void * __malloc_alloc::oom_malloc(size_t n)
{
    void (* my_malloc_handler) ();
    void *result;

    for(;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return result;
    }
}

void * __malloc_alloc::oom_realloc(void *p, size_t n)
{
    void (* my_malloc_handler)();
    void *result;

    for(;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) return result;
    }
}

/**
 * 第二级配置器
 */
enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
class __default_alloc
{
private:
    static size_t ROUND_UP(size_t bytes)            //上调到8的倍数
        { return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1)); }
    union obj
    {
        union obj * free_list_link;
        char client_data[1];
    };
    static obj * volatile free_list[__NFREELISTS];
    static size_t FREELIST_INDEX(size_t bytes)
        { return (((bytes) + __ALIGN - 1)/__ALIGN - 1); }

    static void * refill(size_t n);
    static char * chunk_alloc(size_t size, int &nobjs);

    static char *start_free;
    static char *end_free;
    static size_t heap_size;

public:
    static void * allocate(size_t n);
    static void deallocate(void *p, size_t n);
    static void * reallocate(void *p, size_t old_sz, size_t new_sz);
};

char *__default_alloc::start_free = 0;
char *__default_alloc::end_free = 0;
size_t __default_alloc::heap_size = 0;
__default_alloc::obj * volatile __default_alloc::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void * __default_alloc::allocate(size_t n)
{
    obj * volatile * my_free_list;
    obj * result;
    if (n > static_cast<size_t>(__MAX_BYTES))
        return (__malloc_alloc::allocate(n));
    
    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == 0)
    {
        void *r = refill(ROUND_UP(n));
        return r;
    }
    *my_free_list = result -> free_list_link;
    return result;
}

void __default_alloc::deallocate(void *p, size_t n)
{
    obj *q = static_cast<obj *>(p);
    obj * volatile * my_free_list;

    if (n > static_cast<size_t>(__MAX_BYTES))
    {
        __malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = free_list + FREELIST_INDEX(n);
    q -> free_list_link = *my_free_list;
    *my_free_list = q;
}

void *__default_alloc::refill(size_t n)
{
    int nobjs = 20;
    char * chunk = chunk_alloc(n, nobjs);
    obj * volatile * my_free_list;
    obj * result, * current_obj, * next_obj;

    if (1 == nobjs) return chunk;
    my_free_list = free_list + FREELIST_INDEX(n);

    result = (obj *)chunk;
    *my_free_list = next_obj = (obj *)(chunk + n);
    for(int i = 1;;i++)
    {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (nobjs - 1 == i)
        {
            current_obj -> free_list_link = 0;
            break;
        }
        else
            current_obj -> free_list_link = next_obj;
    }
    return result;
}

char *__default_alloc::chunk_alloc(size_t size, int& nobjs)
{
    char * result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;

    if (bytes_left >= total_bytes)
    {
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else if (bytes_left > size)
    {
        nobjs = bytes_left/size;
        total_bytes = nobjs * size;
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else
    {
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        if (bytes_left > 0)
        {
            //my_free_list不是应该取前一个节点吗？
            obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free) -> free_list_link = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
        start_free = (char *)malloc(bytes_to_get);
        if (0 == start_free)
        {
            obj * volatile * my_free_list, * p;
            for(int i = size;i <= __MAX_BYTES;i += __ALIGN)
            {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (p != 0)
                {
                    *my_free_list = p -> free_list_link;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return (chunk_alloc(size, nobjs));
                }
            }
            end_free = 0;
            start_free = (char *)__malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return (chunk_alloc(size, nobjs));
    }
}
}
#endif