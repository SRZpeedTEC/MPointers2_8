
#pragma once

#include <grpcpp/grpcpp.h>
#include "memory.grpc.pb.h"
#include <mutex>
#include <map>
#include <string>
#include <chrono>
#include <fstream>
using namespace std;

struct memoryBlockInfo  // Creacion de una estructura para representar los bloques de memoria
{
    void* start_ptr;
    size_t size;
    string type;
    int refcount;
};

class memory_manager : public memmgr::MemoryManager::Service{       // Constructor de la clase memory manager, hereda del servicio gRPC para usar las comunicaciones

public:
    memory_manager(void* memoryBlock, size_t totalBytes, string* dumpFolder);

    ~memory_manager() override;

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


    void* memoryBlock;  // Variable para el bloque de memoria principal
    size_t totalBytes;  // Espacio total del bloque
    string* dumpFolder;  // Path al dumpfolder

    map<int, memoryBlockInfo> memoryBlocks; // Estructura para manejar la asignacion de bloques
    int nextId = 1;

    mutex mtx;    // Sincronizacion con el servidor

    void DumpMemory(const string& filename);
    void GarbageCollectorLoop();
};



