
#include "memory_manager.h"
#pragma once
#include <grpcpp/grpcpp.h>
#include "memory.grpc.pb.h"
#include <mutex>
#include <map>
#include <string>
#include <chrono>
#include <fstream>
#include "../MemoryManagerFeatures/dumpFolderFunction.h"
#include "../MemoryManagerFeatures/garbageCollector.h"

using namespace std;


memory_manager::memory_manager(void* memory, size_t totalSize, string* dumpFolder)
    : memoryBlock(memory), totalBytes(totalSize), dumpFolder(dumpFolder), dumpFolderClass(*dumpFolder),
    garbageCollectorMemory(mtx, memoryBlock)
{

}

memory_manager::~memory_manager() = default;

grpc::Status memory_manager::Create(grpc::ServerContext* context,
                                    const memmgr::CreateRequest* request,
                                    memmgr::CreateResponse* response)
{

    std::lock_guard<std::mutex> lock(mtx); // Bloqueamos para que no se hagan asignaciones en simultaneo

    // Obtenemos los parámetros
    uint32_t sizeRequested = request->size(); // tamano reservado para el bloque
    const string& typeRequested = request->type();

    // Agregar situacion para bloques vacios entre espacios usados

    size_t usedBytes = 0;
    for (auto& block : memoryBlocks)
    {
        memoryBlockInfo &blk = block.second;
        char* endOfBlock = static_cast<char*>(blk.start_ptr) + blk.size;
        size_t offsetEnd = endOfBlock - static_cast<char*>(memoryBlock);
        if (offsetEnd > usedBytes)
        {
            usedBytes = offsetEnd;
        }
    }


    // Verificar espacio

    if (usedBytes + sizeRequested > totalBytes)
        return grpc::Status(grpc::StatusCode::RESOURCE_EXHAUSTED, "No hay espacio suficiente");

    int blockId = nextId++;
    memoryBlockInfo newBlockInfo;
    newBlockInfo.start_ptr = static_cast<char*>(memoryBlock) + usedBytes;
    newBlockInfo.size = sizeRequested;
    newBlockInfo.type = typeRequested;
    newBlockInfo.refcount = 1;

    // Actualizamos el map
    memoryBlocks[blockId] = newBlockInfo;

    response->set_id(blockId); // Devolvemos id por medio de response

    // Informamos en consola (opcional)
    cout << "[Create] Nuevo bloque id=" << blockId
    << " rangobytes=[" << usedBytes << "," << usedBytes + newBlockInfo.size << ")"
    << " type=" << typeRequested << " refcount=1" << endl;

    dumpFolderClass.dumpFolderUpdate(memoryBlocks);

    return grpc::Status::OK;

}

grpc::Status memory_manager::Set(grpc::ServerContext* context,
                                 const memmgr::SetRequest* request,
                                 memmgr::SetResponse* response) {

    std::lock_guard<std::mutex> lock(mtx); // Bloqueamos para que no se hagan asignaciones en simultaneo

    uint64_t id = request->id();
    string valueBytes = request->value();

    std::cout << "[Set] Buscando id=" << id
          << " en un map de tamaño=" << memoryBlocks.size() << std::endl;

    auto blockToFind = memoryBlocks.find(static_cast<int>(id));
    if (blockToFind == memoryBlocks.end())
    {
        cerr << "[Set] Bloque no encontrado" << endl;
        response->set_success(false);
        response->set_errormsg("No se encontro el bloque con la id solicitada");
        return grpc::Status::OK;
    }


    memoryBlockInfo& blockInfo = blockToFind->second;

    char* strPtr = static_cast<char*>(blockInfo.start_ptr);
    memcpy(strPtr, valueBytes.data(), valueBytes.size());

    response->set_success(true);
    cout << "[Set] Bloque " << id << " escrito. Bytes=" << valueBytes.size() << endl;

    dumpFolderClass.dumpFolderUpdate(memoryBlocks);

    return grpc::Status::OK;

}

grpc::Status memory_manager::Get(grpc::ServerContext* context,
                                 const memmgr::GetRequest* request,
                                 memmgr::GetResponse* response) {

    std::lock_guard<std::mutex> lock(mtx); // Bloqueamos para que no se hagan asignaciones en simultaneo
    uint64_t id = request->id();
    auto blockToFind = memoryBlocks.find(static_cast<int>(id));
    if (blockToFind == memoryBlocks.end())
    {
        cerr << "[Get] Bloque no encontrado" << endl;
        response->set_success(false);
        response->set_errormsg("No se encontro el bloque con la id solicitada");
        return grpc::Status::OK;
    }

    memoryBlockInfo& blockInfo = blockToFind->second;
    char* strPtr = static_cast<char*>(blockInfo.start_ptr);
    string result(strPtr, strPtr + blockInfo.size);

    response->set_value(result);

    dumpFolderClass.dumpFolderUpdate(memoryBlocks);

    return grpc::Status::OK;

}

grpc::Status memory_manager::IncreaseRefCount(grpc::ServerContext* context,
                                              const memmgr::IncreaseRefCountRequest* request,
                                              memmgr::IncreaseRefCountResponse* response) {

    uint64_t id = request->id();
    auto blockToFind = memoryBlocks.find(static_cast<int>(id));
    if (blockToFind == memoryBlocks.end())
    {
        response->set_success(false);
        response->set_errormsg("No se ha encontrado un bloque con la id solicitada");
    }

    memoryBlockInfo& blockInfo = blockToFind->second;
    blockInfo.refcount++;

    response->set_success(true);
    dumpFolderClass.dumpFolderUpdate(memoryBlocks);

    return grpc::Status::OK;

}

grpc::Status memory_manager::DecreaseRefCount(grpc::ServerContext* context,
                                              const memmgr::DecreaseRefCountRequest* request,
                                              memmgr::DecreaseRefCountResponse* response) {

    uint64_t id = request->id();
    auto blockToFind = memoryBlocks.find(static_cast<int>(id));
    if (blockToFind == memoryBlocks.end())
    {
        response->set_success(false);
        response->set_errormsg("No se ha encontrado un bloque con la id solicitada");
    }

    memoryBlockInfo& blockInfo = blockToFind->second;
    blockInfo.refcount--;

    response->set_success(true);
    dumpFolderClass.dumpFolderUpdate(memoryBlocks);

    return grpc::Status::OK;
}


