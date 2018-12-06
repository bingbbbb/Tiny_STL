#ifndef TINY_STL_LIST_H
#define TINY_STL_LIST_H

#include <iostream>

#include "Tiny_STL_alloc.h"
#include "Tiny_STL_construct.h"
#include "Tiny_STL_uninitialized.h"
#include "Tiny_STL_iterator.h"

namespace Tiny_STL
{
template<class T>
struct __list_node
{
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator
{
    typedef __list_iterator<T, T&, T*>      iterator;
    typedef __list_iterator<T, Ref, Ptr>    self;

    typedef bidirectional_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef Ptr                             pointer;
    typedef Ref                             reference;
    typedef __list_node<T>*                 link_type;
    typedef size_t                          size_type;
    typedef ptrdiff_t                       difference_type;

    link_type node;

    __list_iterator(link_type x) : node(x) {}
    __list_iterator() {}
    __list_iterator(const iterator& x) : node(x.node) {}

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    self& operator++()
    {
        node = (link_type)((*node).next);
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--()
    {
        node = (link_type)((*node).prev);
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template<class T, class Alloc = __default_alloc>
class list
{
public:
    typedef T                               value_type;
	typedef size_t                          size_type;
	typedef T&                              reference;
    typedef __list_iterator<T, T&, T*>      iterator;
    typedef __list_node<T>                  list_node;
    typedef list_node*                      link_type;
    typedef simple_alloc<list_node, Alloc>  list_node_allocator;

    list() { empty_initialize(); }
    ~list()
    {
        iterator iter = begin();
        iterator tmp;
        while(iter != node)
        {
            tmp = iter;
            ++iter;
            destroy_node(tmp.node);
        }
        destroy_node(node);
    }
    iterator begin() { return (link_type)(node->next); }
    iterator end()  { return node; }
    bool empty() const { return node->next == node; }
    size_type size()
    {
        size_type res = 0;
        link_type p = link_type(node->next);
        while(p != node)
        {
            ++res;
            p = link_type(p->next);
        }
        return res;
    }
    reference front() { return *begin(); }
    reference back() { return *(--end()); }
    iterator insert(iterator position, const T& x)
    {
        link_type p = create_node(x);
        p->next = position.node;
        p->prev = position.node->prev;
        (link_type(position.node->prev))->next = p;
        position.node->prev = p;
        return p;
    }
    void push_front(const T& x) { insert(begin(), x); }
    void push_back(const T& x) { insert(end(), x); }
    iterator erase(iterator position)
    {
        link_type next = link_type(position.node->next);
        link_type prev = link_type(position.node->prev);
        next->prev = prev;
        prev->next = next;
        destroy_node(position.node);
        return iterator(next);
    }
    void pop_front() { erase(begin()); }
    void pop_back() 
    {
        iterator tmp = end();
        erase(--tmp);
    }
    void clear();
    void remove(const T& x);
    void splice(iterator position, list& x)
    {
        if (!x.empty())
            transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, list&, iterator i)
    {
        iterator j = i;
        ++j;
        if (position == i || position == j)
            return;
        transfer(position, i, j);
    }
    void splice(iterator position, list&, iterator first, iterator last)
    {
        if (first != last)
            transfer(position, first, last);
    }
    void merge(list<T, Alloc>& x);
    void reverse();
    void sort();
    void swap(list<T, Alloc>& x)
    {
        link_type tmp = node;
        node = x.node;
        x.node = tmp;
    }


private:
    link_type node;

    link_type get_node() { return list_node_allocator::allocate(); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    link_type create_node(const T& x)
    {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }
    void destroy_node(link_type p)
    {
        destroy(&p->data);
        put_node(p);
    }
    void empty_initialize()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
    void transfer(iterator position, iterator first, iterator last);
};

template<class T, class Alloc>
void list<T,Alloc>::clear()
{
    link_type p = begin().node;
    while(p != node)
    {
        p = link_type(p->next);
        destroy_node(link_type(p->prev));
    }
    node->next = node;
    node->prev = node;
}

template<class T, class Alloc>
void list<T,Alloc>::remove(const T& x)
{
    iterator iter = begin();
    iterator end = iterator(node);
    while(iter != end)
    {
        iterator tmp = iter;
        ++iter;
        if (*tmp == x)
            erase(tmp);
    }
}

template<class T, class Alloc>
void list<T,Alloc>::transfer(iterator position, iterator first, iterator last)
{
    (link_type(last.node->prev))->next = position.node;
    (link_type(first.node->prev))->next = last.node;
    (link_type(position.node->prev))->next = first.node;
    link_type tmp = link_type(position.node->prev);
    position.node->prev = last.node->prev;
    last.node->prev = first.node->prev;
    first.node->prev = tmp;
}

template<class T, class Alloc>
void list<T,Alloc>::merge(list<T, Alloc>& x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    iterator cur = end();
    iterator tmp;
    while(first1 != last1 && first2 != last2)
    {
        
        if (*first2 < *first1)
        {
            tmp = first2;
            ++first2;
        }
        else
        {
            tmp = first1;
            ++first1;
        }
        cur.node->next = tmp.node;
        tmp.node->prev = cur.node;
        cur = tmp;
    }
    if (first1 == last1)
    {
        cur.node->next = first2.node;
        first2.node->prev = cur.node;
        tmp = --(x.end());
        tmp.node->next = last1.node;
        last1.node->prev = tmp.node;
    }
    else
    {
        cur.node->next = first1.node;
        first1.node->prev = cur.node;
    }
    last2.node->next = last2.node;
    last2.node->prev = last2.node;
}

template<class T, class Alloc>
void list<T,Alloc>::reverse()
{
    if (node->next == node || link_type(node->next)->next == node)
        return;
    iterator iter = ++(begin());
    while(iter != end())
    {
        iterator old = iter;
        ++iter;
        transfer(begin(), old, iter);
    }
}

template <class T, class Alloc>
void list<T, Alloc>::sort() 
{
    if (node->next == node || link_type(node->next)->next == node) return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) 
        {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);         
        if (i == fill) ++fill;
    } 

    for (int i = 1; i < fill; ++i) 
    {
        counter[i].merge(counter[i-1]);
        //std::cout << "counter[" << i << "]" << counter[i].size() << std::endl;
    }
        
    swap(counter[fill-1]);
}
}
#endif