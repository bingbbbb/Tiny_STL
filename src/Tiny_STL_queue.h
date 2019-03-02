#ifndef TINY_STL_QUEUE_H
#define TINY_STL_QUEUE_H

#include <mutex>     //std::mutex
#include <condition_variable>   //std::condition_variable

#include "my_unique_ptr.h"
#include "my_shared_ptr.h"

namespace Tiny_STL
{
template<class T>
class queue
{
private:
struct node
{
    my_shared_ptr<T>      data;
    my_unique_ptr<node>   next;
};

std::mutex head_m;
std::mutex tail_m;
std::condition_variable cond;
my_unique_ptr<node> head;
node* tail;

public:
    queue() : head(new node), tail(head.get()) {}
    ~queue() {};
    queue(const queue& other) = delete;
    queue& operator=(const queue& other) = delete;
    my_shared_ptr<T> try_pop();
    my_shared_ptr<T> wait_and_pop();
    void push(T x);
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_m);
        std::lock_guard<std::mutex> tail_lock(tail_m);
        return (head.get() == tail);
    }
    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_m);
        return tail;
    }
};
template<class T>
my_shared_ptr<T> queue<T>::try_pop()
{
    my_shared_ptr<T> res;
    {
        std::lock_guard<std::mutex> head_lock(head_m);
        if (head.get() != tail)
        {
            my_unique_ptr<node> old_head = std::move(head);
            head = std::move(old_head->next);
            res = old_head->data;
        }
    }
    return res;
}
template<class T>
my_shared_ptr<T> queue<T>::wait_and_pop()
{
    std::unique_lock<std::mutex> head_lock(head_m);
    while (head.get() == get_tail())        //避免虚假唤醒
        cond.wait(head_lock);
    my_unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head.get()->data;
}
template<class T>
void queue<T>::push(T x)
{
    my_shared_ptr<T> new_data(make_shared<T>(std::move(x)));
    my_unique_ptr<node> p(new node);
    {
        std::lock_guard<std::mutex> tail_lock(tail_m);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = tail->next.get();
    }
    cond.notify_one();
}
}
#endif