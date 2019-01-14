#ifndef TINY_STL_RBTREE_H
#define TINY_STL_RBTREE_H

#include <initializer_list>

#include "Tiny_STL_alloc.h"
#include "Tiny_STL_construct.h"
#include "Tiny_STL_uninitialized.h"
#include "Tiny_STL_iterator.h"
#include "Tiny_STL_function.h"

namespace Tiny_STL
{

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base
{
    __rb_tree_color_type    color;
    __rb_tree_node_base*    parent;
    __rb_tree_node_base*    left;
    __rb_tree_node_base*    right;

static __rb_tree_node_base* minimum(__rb_tree_node_base* x)
{
    while(x->left != nullptr)
        x = x->left;
    return x;
}

static __rb_tree_node_base* maximum(__rb_tree_node_base* x)
{
    while(x->right != nullptr)
        x = x->right;
    return x;
}
};

template<class T>
struct __rb_tree_node : public __rb_tree_node_base
{
    typedef __rb_tree_node<T>*  link_type;
    T value_filed;
};

struct __rb_tree_iterator_base
{
    typedef __rb_tree_node_base*        base_ptr;
    typedef bidirectional_iterator_tag  iterator_category;
    typedef ptrdiff_t                   difference_type;
    base_ptr node;

    void increment()
    {
        if (node->right != nullptr)
        {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        }
        else
        {
            base_ptr y = node->parent;
            while (node == y->right)
            {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
    }

    void decrement()
    {
        if (node->color == __rb_tree_red && node->parent->parent == node)
            node = node->right;
        else if (node->left != nullptr)
        {
            base_ptr y = node->left;
            while (y->right != nullptr)
                y = y->right;
            node = y;
        }
        else
        {
            base_ptr y = node->parent;
            while (node == y->left)
            {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template<class Value, class Ref, class Ptr>
struct __rb_tree_iterator : public __rb_tree_iterator_base
{
    typedef Value                                       value_type;
    typedef Ref                                         reference;
    typedef Ptr                                         pointer;
    typedef __rb_tree_iterator<Value, Value& , Value*>  iterator;
    typedef __rb_tree_iterator<Value, const Value& , const Value*>  const_iterator;
    typedef __rb_tree_iterator<Value, Ref , Ptr>        self;
    typedef __rb_tree_node<Value>*                      link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) { node = x; }
    __rb_tree_iterator(const iterator& it) { node = it.node; }
    
    reference operator*() const { return link_type(node)->value_filed; }
    pointer operator->() const { return &(operator*()); }
    bool operator==(const self& x) { return node==x.node; }
	bool operator!=(const self& x) { return node!=x.node; }
    self& operator++() 
    {
        increment();
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        increment();
        return tmp;
    }
    self& operator--() 
    {
        decrement();
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        decrement();
        return tmp;
    }
};

template<typename T1,typename T2>
struct pair
{
	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;

	pair():first(T1()),second(T2()){}
	pair(const T1& _first,const T2& _second):first(_first),second(_second){}

};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = __malloc_alloc>
class rb_tree
{
public:
    typedef void*                       void_pointer;
    typedef __rb_tree_node_base*        base_ptr;
    typedef __rb_tree_node<Value>       rb_tree_node;
    typedef simple_alloc<rb_tree_node, Alloc>   rb_tree_node_allocator;
    typedef __rb_tree_color_type        color_type;

    typedef Key                         key_type;
    typedef Value                       value_type;
    typedef value_type*                 pointer;
    typedef value_type&                 reference;
    typedef const value_type*           const_pointer;
    typedef const value_type&           const_reference;
    typedef rb_tree_node*               link_type;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef __rb_tree_iterator<value_type, reference, pointer>  iterator;
    typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;
    typedef rb_tree<Key, Value, KeyOfValue, Compare, Alloc> self;

    rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {init();}
    rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) 
    : node_count(0), key_compare(x.key_compare)
    { 
        header = get_node();
        color(header) = __rb_tree_red;
        if (x.root() == nullptr) 
        {
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
        }
        else 
        {
            root() = __copy(x.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count = x.node_count;
    }
    rb_tree(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&& x) : node_count(x.node_count), header(x.header), key_compare(x.key_compare)
    {
        x.init();
        x.node_count = 0;
    }
    rb_tree(std::initializer_list<Value> iniList, const Compare& comp = Compare()) : node_count(0), key_compare(comp) 
    {
        init();
        //auto size = iniList.size();
        //node_count = size;
        for (auto iter = iniList.begin();iter != iniList.end();++iter)
        {
            insert_equal(*iter);
        }
    }
    ~rb_tree()
    {
        clear();
        put_node(header);
    }
    //self& operator=(const self& x);
    Compare key_comp() const { return key_comp; }
    iterator begin() { return leftmost(); }
    iterator end() { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }
    pair<iterator, bool> insert_unique(const value_type& x);
    iterator insert_equal(const value_type& x);
    void clear();
    void erase(iterator x);
    iterator find(const Key& k);
    link_type __copy(link_type x, link_type p);
    

private:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type get_node() { return rb_tree_node_allocator::allocate(); }
    void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
    link_type create_node(const value_type& x)
    {
        link_type p = get_node();
        construct(&p->value_filed, x);
        return p;
    }
    link_type clone_node(link_type x)
    {
        link_type p = create_node(x->value_filed);
        p->color = x->color;
        p->left = nullptr;
        p->right = nullptr;
        return p;
    }
    void destroy_node(link_type p)
    {
        destroy(&p->value_filed);
        put_node(p);
    }

    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }
    static link_type& left(link_type x) { return (link_type&)(x->left); }
    static link_type& right(link_type x) { return (link_type&)(x->right); }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference value(link_type x) { return x->value_filed; }
    static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return x->color; }

    static link_type& left(base_ptr x) { return (link_type&)(x->left); }
    static link_type& right(base_ptr x) { return (link_type&)(x->right); }
    static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
    static reference value(base_ptr x) { return ((link_type)x)->value_filed; }
    static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
    static color_type& color(base_ptr x) { return x->color; }

    static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
    static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }
    iterator __insert(base_ptr x, base_ptr y, const value_type& v);
    //link_type __copy(link_type x, link_type p);
    void recurErase(link_type x);
    void init()
    {
        header = get_node();
        header->color = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
};
}

#include "Tiny_STL_rbtree_impl.h"
#endif