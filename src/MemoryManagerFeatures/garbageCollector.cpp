//
// Created by XPC on 3/24/2025.
//

#include "garbageCollector.h"
#include <thread>
#include <atomic>
#include <iostream>
#include <mutex>
using namespace std;
using namespace std::chrono;

garbageCollector::garbageCollector(mutex& blocksMtx, void* memoryStrt) : blocksMutex(blocksMtx), memoryStart(memoryStrt)

{

}


garbageCollector::~garbageCollector()
{
    stopGarbageCollector();
}

size_t garbageCollector::offSetCalculator(memoryBlockInfo& block)
{
    return static_cast<char*>(block.start_ptr) - static_cast<char*>(memoryStart);
}


void garbageCollector::startGarbageCollector()
{
    garbageCollectorThreadRunning.store(true);
    garbageCollectorThread = thread(garbageCollectorLoop, this);

}
void garbageCollector::stopGarbageCollector()
{
    garbageCollectorThreadRunning.store(false);
    if (garbageCollectorThread.joinable())
    {
        garbageCollectorThread.join();
    }
}

void garbageCollector::garbageCollectorLoop(map<int, memoryBlockInfo>& memoryBlocks)
{
    while (garbageCollectorThreadRunning.load())
    {
        lock_guard<mutex> lock(blocksMutex);
        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end();)
        {
            memoryBlockInfo& block = it->second;
            if (block.refcount <= 0)
            {
                size_t offsetBlocktoDelete = offSetCalculator(block);
                size_t size = block.size;

                cout << "liberando bloque " << it->first << " (refcount = 0)" << endl;
                it = memoryBlocks.erase(it);

                for (auto &blk : memoryBlocks)
                {
                    if (offSetCalculator(blk.second) > offsetBlocktoDelete)
                    {
                        memoryBlockInfo &blockTomove = blk.second;
                        size_t offsetBlocktoMove = offSetCalculator(blockTomove);

                        char* oldPosition = (char*)memoryStart + offsetBlocktoMove;

                        char* newPosition = (char*)memoryStart + (offsetBlocktoMove - size);

                        memmove(newPosition, oldPosition, blockTomove.size);

                        blockTomove.start_ptr = newPosition;
                    }
                }
            }
            else
            {
                ++it;
            }

        }
        this_thread::sleep_for(1s);
    }

}


