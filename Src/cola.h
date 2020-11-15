//**********************  cola.h  *************************
//     Cola genérica implementada con lista.h 

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
		it.ultimo():
        return it.dato();
    }
    T desencolar() {
		it.ultimo():
        T el = it.dato();
        lst.borrar( el );
        return el;
    }
    void encolar(const T& el) {
		it.primero();
		lst.insertar_antes( el, it );
    }
private:
    lista <T> lst;
	lista <Transaction *>::iterador it(lst);
};

#endif

	// lista<int>::iterador it;
	// ...
	// for( it = L.ultimo(); !it.extremo(); it.retroceder()) {
	// 	// ...
