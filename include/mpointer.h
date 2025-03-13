#ifndef MPOINTER_H
#define MPOINTER_H

#include "mock_memory_manager.h"

template <typename T>
class MPointer {
private:
    int id;
    static MockMemoryManager* manager;

    //Esta clase Proxy la hizo chat, tengo que ver bien que es lo que hace xd
    MPointer<T>& operator=(const T&) = delete;

    class Proxy {
    private:
        MPointer<T>* owner;  // Referencia al MPointer para acceder a id y manager
    public:
        // Constructor que guarda el puntero "dueño"
        Proxy(MPointer<T>* p) : owner(p) {}

        // Cuando se lee el valor (por ejemplo std::cout << *p1),
        // este operador de conversión retorna el valor desde manager->Get.
        operator T() const {
            return owner->manager->Get(owner->id);
        }

        // Cuando se escribe (*p1 = X),
        // se invoca este operador= que llama a manager->Set.
        Proxy& operator=(const T& val) {
            owner->manager->Set(owner->id, val);
            return *this;
        }
    };


public:
    static void Init();
    static MPointer<T> New();

    // 2) Devuelve un "Proxy" en lugar de un T&
    Proxy operator*() {
        return Proxy(this);
    }

    // Resto de la clase...
    void operator=(T value);
    void operator=(MPointer<T>& other);
    int operator&();

    ~MPointer();
};
template <typename T>
MockMemoryManager* MPointer<T>::manager = nullptr;

template<typename T>
void MPointer<T>::Init() {
    if (!manager) {
        manager = new MockMemoryManager();
    }
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    int new_id = manager->Create(sizeof(T));
    MPointer<T> ptr;
    ptr.id = new_id;
    return ptr;
}


template<typename T>
void MPointer<T>::operator=(T value) {
    manager->Set(id, value);
}

template <typename T>
void MPointer<T>::operator=(MPointer<T>& other) {
    this->id = other.id;
    manager->IncreaseRefCount(id);
}

template <typename T>
int MPointer<T>::operator&() {
    return id;
}

template <typename T>
MPointer<T>::~MPointer() {
    manager->DecreaseRefCount(id);
}

#endif