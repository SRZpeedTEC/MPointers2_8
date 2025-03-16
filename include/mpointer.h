#ifndef MPOINTER_H
#define MPOINTER_H

#include "mock_memory_manager.h"
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "memory.pb.h"
#include "memory.grpc.pb.h"
#include "../include/mpointerClient.h"

using namespace std;

template <typename T>
class MPointer {
private:
    int id;
    static mpointerClient* client;

    //Esta clase Proxy la hizo chat, tengo que ver bien que es lo que hace xd
    MPointer<T>& operator=(const T&) = delete;

    class Proxy {
    private:
        MPointer<T>* owner;  // Referencia al MPointer para acceder al id y manager
    public:
        // Constructor que guarda el puntero "dueño"
        Proxy(MPointer<T>* p) : owner(p) {}

        template <typename T>
        string SerializeValue(const T& value) {
            return std::string(reinterpret_cast<const char*>(&value), sizeof(value));
        }

        template <typename T>
        T DeserializeValue(const std::string& data) {
            T value;
            std::memcpy(&value, data.data(), sizeof(T));  // Copia los bytes al objeto
            return value;
        }


        // Cuando se lee el valor (por ejemplo std::cout << *p1),
        // este operador de conversión retorna el valor desde manager->Get.
        operator T() const {
            string binValue = owner->client->HandleGetRequest(owner->id);
            T value = DeserializeValue<T>(binValue);
            return value;
        }

        // Cuando se escribe (*p1 = X),
        // se invoca este operador= que llama a manager->Set.
        Proxy& operator=(const T& val) {
            string binValue = SerializeValue(val);
            owner->client->HandleSetRequest(owner->id, binValue);
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
string SerializeValue(const T& value) {
    return std::string(reinterpret_cast<const char*>(&value), sizeof(value));
}


template <typename T>
mpointerClient* MPointer<T>::client = nullptr;


template<typename T>
void MPointer<T>::Init() {
    if (!client) {
        client = new mpointerClient();
        client -> initiateClient();
    }
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    string type = typeid(T).name();
    size_t size = sizeof(T);
    int new_id = client->HandleCreateRequest(type,size);
    MPointer<T> ptr;
    ptr.id = new_id;
    return ptr;
}


template<typename T>
void MPointer<T>::operator=(T value) {
    string binValue = Serialize<T>(value);
    client->HandleSetRequest(id, binValue);
}

template <typename T>
void MPointer<T>::operator=(MPointer<T>& other) {
    this->id = other.id;
    client->HandleIncreaseRefCountRequest(id);
}

template <typename T>
int MPointer<T>::operator&() {
    return id;
}

template <typename T>
MPointer<T>::~MPointer() {
    client->HandleDecreaseRefCountRequest(id);
}



#endif