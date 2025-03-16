#include <iostream>
#include <grpcpp/grpcpp.h>
#include "memory.pb.h"
#include "memory.grpc.pb.h"
#include "../include/mpointerClient.h"
#include <iostream>




void mpointerClient::initiateClient() {
    channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    stub = memmgr::MemoryManager::NewStub(channel);

}


int mpointerClient::HandleCreateRequest(string type, size_t size)
{

    memmgr::CreateRequest req;

    req.set_size(size);
    req.set_type(type);

    memmgr::CreateResponse res;
    grpc::ClientContext ctx;

    grpc::Status status = stub->Create(&ctx, req, &res);
    if (!status.ok()) {
        std::cerr << "Create falló. Code=" << status.error_code()
                  << " msg=" << status.error_message() << std::endl;
        return 1;
    }
    std::cout << "[CLIENT] Create exitoso, id=" << res.id() << std::endl;
    return res.id();


}

bool mpointerClient::HandleSetRequest(int id, string binValue)
{
    memmgr::SetRequest request;
    request.set_id(id);
    request.set_value(binValue);

    memmgr::SetResponse sres; // Almacena la respuesta
    grpc::ClientContext sctx;
    grpc::Status stSet = stub->Set(&sctx, request, &sres);
    if (!stSet.ok()) {
        std::cerr << "Set falló (RPC error). Code=" << stSet.error_code()
                  << " msg=" << stSet.error_message() << std::endl;
        return 1;
    }
    if (!sres.success()) {
        std::cerr << "Set falló lógicamente: " << sres.errormsg() << std::endl;
        return 1;
    }
    std::cout << "[CLIENT] Set exitoso en bloque " << id << std::endl;
    return 0;
}


string mpointerClient::HandleGetRequest(int id)
{
    memmgr::GetRequest request;
    request.set_id(id);

    memmgr::GetResponse gres;
    grpc::ClientContext gctx;
    grpc::Status stGet = stub->Get(&gctx, request, &gres);
    if (!stGet.ok()) {
        std::cerr << "Get falló (RPC error). Code=" << stGet.error_code()
                  << " msg=" << stGet.error_message() << std::endl;
        return "error";
    }
    else {
        std::string data = gres.value();
        //std::cout << "[CLIENT] Valor leído: " << data << std::endl;
        return data;
    }


}


bool mpointerClient::HandleIncreaseRefCountRequest(int id)
{
    memmgr::IncreaseRefCountRequest request;
    request.set_id(id);

    memmgr::IncreaseRefCountResponse Ires; // Almacena la respuesta
    grpc::ClientContext Ictx;
    grpc::Status stInRefCt = stub->IncreaseRefCount(&Ictx, request, &Ires);
    if (!stInRefCt.ok()) {
        std::cerr << "Increase Ref Count falló (RPC error). Code=" << stInRefCt.error_code()
                  << " msg=" << stInRefCt.error_message() << std::endl;
        return 1;
    }
    if (!Ires.success()) {
        std::cerr << "Increase Ref Count falló lógicamente: " << Ires.errormsg() << std::endl;
        return 1;
    }
    std::cout << "[CLIENT] Increase Ref Count exitoso en bloque " << id << std::endl;
    return 0;


}

bool mpointerClient::HandleDecreaseRefCountRequest(int id)
{
    memmgr::DecreaseRefCountRequest request;
    request.set_id(id);

    memmgr:: DecreaseRefCountResponse Dres; // Almacena la respuesta
    grpc::ClientContext Dctx;
    grpc::Status stDeRefCt = stub->DecreaseRefCount(&Dctx, request, &Dres);
    if (!stDeRefCt.ok()) {
        std::cerr << "Decrease Ref Count falló (RPC error). Code=" << stDeRefCt.error_code()
                  << " msg=" << stDeRefCt.error_message() << std::endl;
        return 1;
    }
    if (!Dres.success()) {
        std::cerr << "Decrease Ref Count falló lógicamente: " << Dres.errormsg() << std::endl;
        return 1;
    }
    std::cout << "[CLIENT] Decrease Ref Count exitoso en bloque " << id << std::endl;
    return 0;




}



/*
int Pruebas() {
    // Crear canal y stub


    // ============ CREAR BLOQUE ============
    {

        // ============ SET ============
        {
            memmgr::SetRequest sreq;
            sreq.set_id(res.id());

            // Guardar el int 2023
            std::string valor = "hola";
            std::string binValue(reinterpret_cast<const char*>(&valor), sizeof(valor));
            sreq.set_value("hola");

            memmgr::SetResponse sres;
            grpc::ClientContext sctx;
            grpc::Status stSet = stub->Set(&sctx, sreq, &sres);
            if (!stSet.ok()) {
                std::cerr << "Set falló (RPC error). Code=" << stSet.error_code()
                          << " msg=" << stSet.error_message() << std::endl;
                return 1;
            }
            if (!sres.success()) {
                std::cerr << "Set falló lógicamente: " << sres.errormsg() << std::endl;
                return 1;
            }
            std::cout << "[CLIENT] Set exitoso en bloque " << res.id() << std::endl;
        }

        // ============ GET ============
        {
            memmgr::GetRequest greq;
            greq.set_id(res.id());

            memmgr::GetResponse gres;
            grpc::ClientContext gctx;
            grpc::Status stGet = stub->Get(&gctx, greq, &gres);
            if (!stGet.ok()) {
                std::cerr << "Get falló (RPC error). Code=" << stGet.error_code()
                          << " msg=" << stGet.error_message() << std::endl;
                return 1;
            }
            // Extraer 4 bytes
            if (gres.value().size() == 4) {
                std::string data = gres.value();
                std::cout << "[CLIENT] Valor leído: " << data << std::endl;
            } else {
                std::cerr << "[CLIENT] Error: tamaño de datos != 4" << std::endl;
            }
        }
    }

    return 0;
}
*/