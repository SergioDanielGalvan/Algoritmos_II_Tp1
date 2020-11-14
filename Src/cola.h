//**********************  cola.h  *************************
//     Cola generica implementada con lista.h 

#ifndef COLA_H_
#define COLA_H_

#include <lista.h>

template <class T>
class Cola {
public:
    Cola() { 
    }
    void clear() {
        lst.borrartodo();
    }
    bool isEmpty() const { 
        return lst.vacia();  
    }
    T& front() { 
        return lst.front();  
    }
    T desencolar() {
        T el = lst.front();
        lst.pop_front();
        return el;
    }
    void encolar(const T& el) {
        lst.push_back(el);
    }
private:
    lista <T> lst;
	lista <Transaction *>::iterador it(lst);
};

#endif
