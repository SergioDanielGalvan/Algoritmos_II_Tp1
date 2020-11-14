//**********************  cola.h  *************************
//     Cola generica implementada con lista.h 

#ifndef COLA_H_
#define COLA_H_

#include <lista.h>


template <class T>
class Queue {
public:
    Queue() { 
    }
    void clear() {
        lst.clear();
    }
    bool isEmpty() const { 
        return lst.empty();  
    }
    T& front() { 
        return lst.front();  
    }
    T dequeue() {
        T el = lst.front();
        lst.pop_front();
        return el;
    }
    void enqueue(const T& el) {
        lst.push_back(el);
    }
private:
    lista <T> lst;
};

#endif
