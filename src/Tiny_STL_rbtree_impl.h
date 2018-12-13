#ifndef TINY_STL_RBTREE_IMPL_H
#define TINY_STL_RBTREE_IMPL_H

#include <iostream>
//#include "Tiny_STL_rbtree.h"
namespace Tiny_STL
{
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
{
    link_type y = header;
    link_type x = root();
    while (x != nullptr)
    {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x); 
    }
    return __insert(x, y, v);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
{
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }
    iterator j = iterator(y);  
    if (comp)
        if (j == begin())     
            return pair<iterator,bool>(__insert(x, y, v), true);
        else
            --j;
    if (key_compare(key(j.node), KeyOfValue()(v)))
        return pair<iterator,bool>(__insert(x, y, v), true);
    return pair<iterator,bool>(j, false);
}

inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    __rb_tree_node_base* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    __rb_tree_node_base* y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}


inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
{
    x->color = __rb_tree_red;
    while (x != root && x->parent->color == __rb_tree_red) 
    {
        if (x->parent == x->parent->parent->left) 
        {
            __rb_tree_node_base* y = x->parent->parent->right;
            if (y && y->color == __rb_tree_red) 
            {
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            }
            else 
            {
                if (x == x->parent->right) 
                {
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else {
        __rb_tree_node_base* y = x->parent->parent->left;
        if (y && y->color == __rb_tree_red) {
            x->parent->color = __rb_tree_black;
            y->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            x = x->parent->parent;
        }
        else {
            if (x == x->parent->left) {
            x = x->parent;
            __rb_tree_rotate_right(x, root);
            }
            x->parent->color = __rb_tree_black;
            x->parent->parent->color = __rb_tree_red;
            __rb_tree_rotate_left(x->parent->parent, root);
        }
        }
    }
    root->color = __rb_tree_black;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v)
{
    link_type x = (link_type) x_;
    link_type y = (link_type) y_;
    link_type z;

    if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v);
        left(y) = z;              
        if (y == header) 
        {
            root() = z;
            rightmost() = z;
        }
        else if (y == leftmost())
            leftmost() = z;           
    }
    else {
        z = create_node(v);
        right(y) = z;
        if (y == rightmost())
            rightmost() = z;          
    }
    parent(z) = y;
    left(z) = nullptr;
    right(z) = nullptr;
    __rb_tree_rebalance(z, header->parent);
    ++node_count;
    //std::cout << "z" << z->value_filed << std::endl;
    return iterator(z);
}



template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
{
    recurErase(root());
    left(header) = header;
    right(header) = header;
    parent(header) = nullptr;
}

inline __rb_tree_node_base*
__rb_tree_rebalance_for_erase(__rb_tree_node_base* z,
                              __rb_tree_node_base*& root,
                              __rb_tree_node_base*& leftmost,
                              __rb_tree_node_base*& rightmost)
{
    __rb_tree_node_base* y = z;
    __rb_tree_node_base* x = nullptr;
    __rb_tree_node_base* x_parent = nullptr;
    if (y->left == nullptr)             // z has at most one non-null child. y == z.
        x = y->right;               // x might be null.
    else
        if (y->right == nullptr)          // z has exactly one non-null child.  y == z.
            x = y->left;              // x is not null.
        else {                      // z has two non-null children.  Set y to
            y = y->right;             //   z's successor.  x might be null.
            while (y->left != nullptr)
                y = y->left;
            x = y->right;
        }
    if (y != z) {                 // relink y in place of z.  y is z's successor
        z->left->parent = y; 
        y->left = z->left;
        if (y != z->right) {
        x_parent = y->parent;
        if (x) x->parent = y->parent;
        y->parent->left = x;      // y must be a left child
        y->right = z->right;
        z->right->parent = y;
        }
        else
        x_parent = y;  
        if (root == z)
        root = y;
        else if (z->parent->left == z)
        z->parent->left = y;
        else 
        z->parent->right = y;
        y->parent = z->parent;
        //__STD::swap(y->color, z->color);
        __rb_tree_color_type tmp = y->color;
        y->color = z->color;
        z->color = tmp;
        y = z;
        // y now points to node to be actually deleted
    }
    else {                        // y == z
        x_parent = y->parent;
        if (x) x->parent = y->parent;   
        if (root == z)
        root = x;
        else 
        if (z->parent->left == z)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (leftmost == z) 
        if (z->right == nullptr)        // z->left must be null also
            leftmost = z->parent;
        // makes leftmost == header if z == root
        else
            leftmost = __rb_tree_node_base::minimum(x);
        if (rightmost == z)  
        if (z->left == nullptr)         // z->right must be null also
            rightmost = z->parent;  
        // makes rightmost == header if z == root
        else                      // x == z->left
            rightmost = __rb_tree_node_base::maximum(x);
    }
    if (y->color != __rb_tree_red) { 
        while (x != root && (x == nullptr || x->color == __rb_tree_black))
        if (x == x_parent->left) {
            __rb_tree_node_base* w = x_parent->right;
            if (w->color == __rb_tree_red) {
                w->color = __rb_tree_black;
            x_parent->color = __rb_tree_red;
            __rb_tree_rotate_left(x_parent, root);
            w = x_parent->right;
        }
        if ((w->left == nullptr || w->left->color == __rb_tree_black) &&
            (w->right == nullptr || w->right->color == __rb_tree_black)) {
            w->color = __rb_tree_red;
            x = x_parent;
            x_parent = x_parent->parent;
        } else {
            if (w->right == nullptr || w->right->color == __rb_tree_black) {
                if (w->left) w->left->color = __rb_tree_black;
                    w->color = __rb_tree_red;
                __rb_tree_rotate_right(w, root);
                w = x_parent->right;
            }
            w->color = x_parent->color;
            x_parent->color = __rb_tree_black;
            if (w->right) w->right->color = __rb_tree_black;
                __rb_tree_rotate_left(x_parent, root);
            break;
            }
        } else {                  // same as above, with right <-> left.
            __rb_tree_node_base* w = x_parent->left;
            if (w->color == __rb_tree_red) {
            w->color = __rb_tree_black;
            x_parent->color = __rb_tree_red;
            __rb_tree_rotate_right(x_parent, root);
            w = x_parent->left;
            }
            if ((w->right == nullptr || w->right->color == __rb_tree_black) &&
                (w->left == nullptr || w->left->color == __rb_tree_black)) {
            w->color = __rb_tree_red;
            x = x_parent;
            x_parent = x_parent->parent;
            } else {
                if (w->left == nullptr || w->left->color == __rb_tree_black) {
                    if (w->right) w->right->color = __rb_tree_black;
                w->color = __rb_tree_red;
                __rb_tree_rotate_left(w, root);
                w = x_parent->left;
            }
            w->color = x_parent->color;
            x_parent->color = __rb_tree_black;
            if (w->left) w->left->color = __rb_tree_black;
                __rb_tree_rotate_right(x_parent, root);
            break;
            }
        }
        if (x) x->color = __rb_tree_black;
    }
    return y;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator x)
{
    link_type y = (link_type) __rb_tree_rebalance_for_erase(x.node, header->parent, header->left, header->right);
    destroy_node(y);
    --node_count;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::recurErase(link_type x)
{
    if (x != nullptr)
    {
        recurErase(left(x));
        recurErase(right(x));
        destroy_node(x);
    }
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k)
{
    link_type y = header;
    link_type x = root();

    while (x != nullptr) 
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);

    iterator j = iterator(y);   
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) 
{
    link_type top = clone_node(x);
    top->parent = p;
    if (x->right)
        top->right = __copy(right(x), top);
    p = top;
    x = left(x);
    while (x != nullptr) 
    {
        link_type y = clone_node(x);
        p->left = y;
        y->parent = p;
        if (x->right)
            y->right = __copy(right(x), y);
        p = y;
        x = left(x);
    }
    return top;
}

}
#endif