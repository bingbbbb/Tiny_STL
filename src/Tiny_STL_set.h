#ifndef TINY_STL_SET_H
#define TINY_STL_SET_H

#include <initializer_list>

#include "Tiny_STL_alloc.h"
#include "Tiny_STL_construct.h"
#include "Tiny_STL_uninitialized.h"
#include "Tiny_STL_iterator.h"
//#include "Tiny_STL_function.h"
#include "Tiny_STL_rbtree.h"

namespace Tiny_STL
{
template<class Key, class Compare = less<Key>, class Alloc = __default_alloc>
class set
{
public:
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
    typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::size_type size_type;

    set() : t(Compare()) {}
    set(const set<Key, Compare, Alloc>& x) : t(x.t) {}
    set(set<Key, Compare, Alloc>&& x) : t(std::move(x.t)) {}
    set(std::initializer_list<Key> iniList) : t(iniList) {}
    explicit set(const Compare& comp) : t(comp) {}
    iterator begin() { return t.begin(); }
    iterator end() { return t.end(); }
    bool empty() { return t.empty(); }
    size_type size() { return t.size(); }
    size_type max_size() { return t.max_size(); }
    pair<iterator, bool> insert(const value_type& x)
    {
        //因为iterator是const，所以多了一步转换
        pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
		return pair<iterator,bool>(p.first, p.second);
    }
    void erase(iterator position)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)position);
    }
    iterator find(const key_type& x) { return t.find(x); }

private:
    rep_type t;

};
}
#endif