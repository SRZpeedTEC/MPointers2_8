#include <iostream>
#include <grpcpp/grpcpp.h>
#include "memory.pb.h"
#include "memory.grpc.pb.h"

int main() {
    // Crear canal y stub
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    auto stub = memmgr::MemoryManager::NewStub(channel);

    // ============ CREAR BLOQUE ============
    {
        memmgr::CreateRequest req;
        req.set_size(4);       // Bloque de 4 bytes
        req.set_type("string");

        memmgr::CreateResponse res;
        grpc::ClientContext ctx;

        grpc::Status status = stub->Create(&ctx, req, &res);
        if (!status.ok()) {
            std::cerr << "Create falló. Code=" << status.error_code()
                      << " msg=" << status.error_message() << std::endl;
            return 1;
        }
        std::cout << "[CLIENT] Create exitoso, id=" << res.id() << std::endl;

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
