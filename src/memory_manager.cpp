//
// Created by XPC on 3/8/2025.
//

#include "memory_manager.h"

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

// Cabeceras generadas a partir de hello.proto
#include "memory.pb.h"
#include "memory.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Nombres de paquete en .proto
using greet::Greeter;
using greet::HelloRequest;
using greet::HelloReply;

class GreeterServiceImpl final : public Greeter::Service {
public:
    Status SayHello(ServerContext* context,
                    const HelloRequest* request,
                    HelloReply* reply) override {
        // Implementación de la lógica
        std::string name = request->name();
        reply->set_message("Hola " + name);
        return Status::OK;
    }
};

int main() {
    // Direccion del servidor
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    // Configurar y construir el servidor
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Servidor escuchando en " << server_address << std::endl;

    // Mantener el servidor activo
    server->Wait();
    return 0;
}
