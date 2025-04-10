#ifndef MPOINTER_H
#define MPOINTER_H

#include "mock_memory_manager.h"
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "memory.pb.h"
#include "memory.grpc.pb.h"
#include "../include/mpointerClient.h"

using namespace std;
static mpointerClient* globalClient = nullptr;


template <typename T>
class MPointer {
private:


    class Proxy {
    private:
        MPointer<T>* owner;
    public:
        Proxy(MPointer<T>* p) : owner(p) {}

        string SerializeValue(const T& value) {
            return std::string(reinterpret_cast<const char*>(&value), sizeof(value));
        }

        T DeserializeValue(const std::string& data) const {
            T value;
            std::memcpy(&value, data.data(), sizeof(T));
            return value;
        }

        operator T() const {
            string binValue = owner->getClient()->HandleGetRequest(owner->id);
            T value = DeserializeValue(binValue);
            return value;
        }

        Proxy& operator=(const T& val) {
            string binValue = SerializeValue(val);
            owner->getClient()->HandleSetRequest(owner->id, binValue);
            return *this;
        }
    };

public:
    int id;
    static void Init();
    static MPointer<T> New();

    Proxy operator*() & {
        return Proxy(this);
    }

    void operator=(T value);
    void operator=(MPointer<T>& other);
    int operator&();

    ~MPointer();



    static mpointerClient* getClient() {
        return globalClient;
    }
};




template<typename T>
void MPointer<T>::Init() {
    if (!globalClient) {
        globalClient = new mpointerClient();
        globalClient->initiateClient();
    }
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    if (!globalClient) {
        Init();
    }

    string type = typeid(T).name();
    size_t size = sizeof(T);
    int new_id = globalClient->HandleCreateRequest(type, size);

    MPointer<T> ptr;
    ptr.id = new_id;
    return ptr;
}

template<typename T>
void MPointer<T>::operator=(T value) {
    string binValue = SerializeValue(value);
    globalClient->HandleSetRequest(id, binValue);
}

template <typename T>
void MPointer<T>::operator=(MPointer<T>& other) {
    if (id != &other) {
        if (id != 0) {
            globalClient->HandleDecreaseRefCountRequest(id);
        }
        id = other.id;
        globalClient->HandleIncreaseRefCountRequest(id);
    }
}

template <typename T>
int MPointer<T>::operator&() {
    return id;
}

template <typename T>
MPointer<T>::~MPointer() {
    if (id != 0) {
        globalClient->HandleDecreaseRefCountRequest(id);
    }
}

#endif