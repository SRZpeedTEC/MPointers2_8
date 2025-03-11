
#pragma once

#include <grpcpp/grpcpp.h>
#include "memory.grpc.pb.h"
#include <mutex>
#include <map>
#include <string>
#include <chrono>
#include <fstream>
using namespace std;

struct memoryblockinfo
{
    size_t start_size;
    size_t end_size;
    string type;
    int refcount;
};

class memory_manager : public memmgr::MemoryManager::Service{

public:
    memory_manager(void* memoryBlock, size_t totalBytes, string dumpfolder);
    ~memory_manager();
    grpc::Status Create(grpc::ServerContext* context,
                         const memmgr::CreateRequest* request,
                         memmgr::CreateResponse* response) override;

    grpc::Status Set(grpc::ServerContext* context,
                     const memmgr::SetRequest* request,
                     memmgr::SetResponse* response) override;

    grpc::Status Get(grpc::ServerContext* context,
                     const memmgr::GetRequest* request,
                     memmgr::GetResponse* response) override;

    grpc::Status IncreaseRefCount(grpc::ServerContext* context,
                                  const memmgr::IncreaseRefCountRequest* request,
                                  memmgr::IncreaseRefCountResponse* response) override;

    grpc::Status DecreaseRefCount(grpc::ServerContext* context,
                                  const memmgr::DecreaseRefCountRequest* request,
                                  memmgr::DecreaseRefCountResponse* response) override;


    void* memoryBlock;
    size_t totalBytes;
    string dumpfolder;

    map<int, memoryblockinfo> memoryblocks;
    int nextId = 1;

    mutex mutex;
};



