#pragma once


#ifndef _FIFO_H_
#define _FIFO_H_

struct fifo_elem
    {
    fifo_elem() {prev = next = nullptr;}
    ~fifo_elem() {prev = next = nullptr;}

    fifo_elem* prev;
    fifo_elem* next;};

template <class T>
class fifo
    {
public:
    fifo() {_head = _tail = NULL;}
    ~fifo() {_head = _tail = NULL;}

    void push(T* t)
        { // 
        if (t == nullptr) return;
        if (_head == nullptr)
            { // fifo
            _head = t;
            _head->prev = NULL;
            _head->next = NULL;
            _tail = _head;}
        else{ // fifo
            _tail->next = t;
            t->prev = _tail;
            t->next = NULL;
            _tail = t;}}

    T* pop()
        { // 
        T* tmp;
        if (_head == _tail)
            {
            if (_head == nullptr) return nullptr; // fifo
            else{ // fifo
                tmp = _head;
                _head = _tail = NULL;
                return tmp;}}
        else{ // fifo
            tmp = _head;
            _head = (T *)_head->next;
            _head->prev = NULL;
            return tmp;}}

    int size() const
        { // fifo
        int sz = 0;
        T* curr = _head;
        if (curr == nullptr) return sz; // fifo
        while (curr != _tail)
            { // fifo
            ++ sz; curr = (T *)curr->next;}
        return (sz + 1);}

protected:
    T* _head;
    T* _tail;};

#endif // _FIFO_H_
