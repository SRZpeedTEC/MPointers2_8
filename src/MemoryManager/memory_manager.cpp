
#include "memory_manager.h"
#pragma once

#include <grpcpp/grpcpp.h>
#include "memory.grpc.pb.h"
#include <mutex>
#include <map>
#include <string>
#include <chrono>
#include <fstream>


memory_manager::memory_manager(void* memory, size_t totalSize, const string dumpFolder)
    : memoryBlock(memory), totalBytes(totalSize), dumpfolder(dumpFolder)
{

}

memory_manager::~memory_manager() {

}