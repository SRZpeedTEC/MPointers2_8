//
// Created by XPC on 3/8/2025.
//



#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "memory.pb.h"
#include "memory.grpc.pb.h"
#include "mpointer.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using greet::Greeter;
using greet::HelloRequest;
using greet::HelloReply;


class GreeterClient {
public:
    // Recibe un canal para conectarse al servidor
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    std::string SayHello(const std::string& user) {
        HelloRequest request;
        request.set_name(user);

        HelloReply reply;
        ClientContext context;

        // Llamada RPC
        Status status = stub_->SayHello(&context, request, &reply);
        if (status.ok()) {
            return reply.message();
        } else {
            std::cerr << "Error en RPC: " << status.error_message() << std::endl;
            return {};
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main() {
    // Conectarse al servidor en localhost:50051
    GreeterClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

    // Llamar a SayHello
    std::string response = client.SayHello("Mundo");
    std::cout << "Respuesta del servidor: " << response << std::endl;
    return 0;
}

