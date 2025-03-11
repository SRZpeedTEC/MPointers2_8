cmake_minimum_required(VERSION 3.10)
project(MPointers2_8)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

message(STATUS "Protobuf version: ${Protobuf_VERSION}")
message(STATUS "gRPC plugin: ${gRPC_CPP_PLUGIN}")

# 1) Archivo .proto
set(PROTO_FILE "${CMAKE_SOURCE_DIR}/proto/memory.proto")

# 2) Directorio donde se generan .pb.* y .grpc.pb.*
#    Se recomienda usar una carpeta dentro de la build (ej. CMAKE_CURRENT_BINARY_DIR),
#    pero si prefieres, puedes dejarlos en ${CMAKE_SOURCE_DIR}/proto.
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# 3) Definir nombres de los archivos de salida
get_filename_component(PROTO_BASENAME ${PROTO_FILE} NAME_WE)
set(PB_SRC   "${GENERATED_DIR}/${PROTO_BASENAME}.pb.cc")
set(PB_HDR   "${GENERATED_DIR}/${PROTO_BASENAME}.pb.h")
set(GRPC_SRC "${GENERATED_DIR}/${PROTO_BASENAME}.grpc.pb.cc"
        src/memory_manager_server.h)
set(GRPC_HDR "${GENERATED_DIR}/${PROTO_BASENAME}.grpc.pb.h")

# 4) Comando para invocar protoc
add_custom_command(
        OUTPUT "${PB_SRC}" "${PB_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS
        --proto_path=${CMAKE_SOURCE_DIR}/proto
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc="E:/ClionProjects/vcpkg/installed/x64-windows/tools/grpc/grpc_cpp_plugin.exe" # "/Users/santirobles/vcpkg/installed/arm64-osx/tools/grpc/grpc_cpp_plugin"
        memory.proto
)

# 5) Crear la librería con los archivos generados
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

# 6) Ejecutable del servidor (ejemplo)
add_executable(server
        src/memory_manager_server.cpp
        # Puedes añadir más .cpp de tu servidor
)
target_link_libraries(server PRIVATE memory_proto)

# 7) Ejecutable del cliente (ejemplo)
add_executable(client
        src/mpointer.cpp
        # Más .cpp si deseas
)
target_link_libraries(client PRIVATE memory_proto)
