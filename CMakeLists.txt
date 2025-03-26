cmake_minimum_required(VERSION 3.10)
project(MPointers2_8)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

message(STATUS "Protobuf version: ${Protobuf_VERSION}")
message(STATUS "gRPC plugin: ${gRPC_CPP_PLUGIN}")

# Archivo .proto
set(PROTO_FILE "${CMAKE_SOURCE_DIR}/proto/memory.proto")

# Directorio donde se generan .pb.* y .grpc.pb.*

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Definir nombres de los archivos de salida
get_filename_component(PROTO_BASENAME ${PROTO_FILE} NAME_WE)
set(PB_SRC   "${GENERATED_DIR}/${PROTO_BASENAME}.pb.cc")
set(PB_HDR   "${GENERATED_DIR}/${PROTO_BASENAME}.pb.h")
set(GRPC_SRC "${GENERATED_DIR}/${PROTO_BASENAME}.grpc.pb.cc"
        include/mpointerClient.h
        src/MemoryManagerFeatures/dumpFolderFunction.cpp
        src/MemoryManagerFeatures/dumpFolderFunction.h
        src/MemoryManager/memoryBlockInfo.h
        src/MemoryManagerFeatures/garbageCollector.cpp
        src/MemoryManagerFeatures/garbageCollector.h
)
set(GRPC_HDR "${GENERATED_DIR}/${PROTO_BASENAME}.grpc.pb.h")

# Comando para invocar protoc
add_custom_command(
        OUTPUT "${PB_SRC}" "${PB_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS
        --proto_path=${CMAKE_SOURCE_DIR}/proto
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        #--plugin=protoc-gen-grpc="/Users/santirobles/vcpkg/installed/arm64-osx/tools/grpc/grpc_cpp_plugin"
        --plugin=protoc-gen-grpc="E:/ClionProjects/vcpkg/installed/x64-windows/tools/grpc/grpc_cpp_plugin.exe"
        memory.proto
)

# Crear la librería con los archivos generados
add_library(memory_proto
        "${PB_SRC}"
        "${GRPC_SRC}"
)

# Incluir la carpeta de archivos generados (para encontrar .pb.h y .grpc.pb.h)
target_include_directories(memory_proto
        PUBLIC
        "${GENERATED_DIR}"
)

# Vincular con las librerías de gRPC y Protobuf
target_link_libraries(memory_proto
        PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
)

# Ejecutable del servidor
add_executable(server
        src/memory_manager_server.cpp
        src/MemoryManager/memory_manager.cpp

)
target_link_libraries(server PRIVATE memory_proto)

#  Ejecutable del cliente
add_executable(client
        test/test_mpointers.cpp
        src/mpointerClient.cpp

)
target_link_libraries(client PRIVATE memory_proto)