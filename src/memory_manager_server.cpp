#include "memory_manager_server.h"
#include "MemoryManager/memory_manager.h"
#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include <thread>
#include "memory.pb.h"
#include "memory.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace std;


int main(int argc, char** argv) {

    string port;
    size_t memsize = 0;
    string dumpfolder;

    for (int i = 1; i < argc; i++){
        string arg = argv[i];
        try {
            if (arg == "--port") {
                if (i + 1 < argc) port = argv[++i];
                else throw invalid_argument("Falta valor para --port");
            } else if (arg == "--memsize") {
                if (i + 1 < argc) memsize = stoi(argv[++i]);
                else throw invalid_argument("Falta valor para --memsize");
            } else if (arg == "--dumpFolder") {
                if (i + 1 < argc) dumpfolder = argv[++i];
                else throw invalid_argument("Falta valor para --dumpFolder");
            } else {
                cerr << "Argumento desconocido: " << arg << endl;
            }
        } catch (const exception& e) {
            cerr << "Error procesando argumentos: " << e.what() << endl;
            return 1;
        }
    }
    size_t totalBytes = memsize * 1024 * 1024;
    void* memoryBlock = malloc(totalBytes);
    if (!memoryBlock)
    {
        cerr << "Error al asignar memoria" << endl;
        return 1;
    }

    memory_manager service(memoryBlock, totalBytes, dumpfolder);

    string server_address("0.0.0.0:" + port);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());

    cout << "Configuración final:" << endl;
    cout << " - Puerto: " << port << endl;
    cout << " - Tamaño de memoria: " << memsize << "MB" << endl;
    cout << " - Carpeta de dumps: " << dumpfolder << endl;
    cout << "Servidor iniciado en " << server_address << endl;


    free(memoryBlock);
    return 0;
}


