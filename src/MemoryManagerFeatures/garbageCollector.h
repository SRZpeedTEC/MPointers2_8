//
// Created by XPC on 3/24/2025.
//

#ifndef GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_H
#include <string>
#include <map>
#include "../MemoryManager/memoryBlockInfo.h"
#include <thread>
#include <atomic>
#include <mutex>


using namespace std;


class garbageCollector {

public:
    garbageCollector(mutex& blocksMutex, void* memoryStart);
    ~garbageCollector();

    size_t offSetCalculator(memoryBlockInfo& block);

    void startGarbageCollector();

    void stopGarbageCollector();

    void garbageCollectorLoop(map<int, memoryBlockInfo>& memoryBlocks);

    mutex& blocksMutex;
    void* memoryStart;

private:

    thread garbageCollectorThread;
    atomic<bool> garbageCollectorThreadRunning;


};



#endif //GARBAGECOLLECTOR_H
