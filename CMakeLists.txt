cmake_minimum_required(VERSION 3.30)
project(MPointers2_8)

set(CMAKE_CXX_STANDARD 20)


# (Opcional) si usas vcpkg y no está integrado globalmente, fuerza el toolchain:
# set(CMAKE_TOOLCHAIN_FILE "C:/ruta/a/vcpkg/scripts/buildsystems/vcpkg.cmake")

# 1) Buscar Protobuf y gRPC (versión "CONFIG" o la que uses con vcpkg):
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
message(STATUS "gRPC plugin: ${gRPC_CPP_PLUGIN}")


# 2) Archivo .proto que queremos compilar:
set(PROTO_FILE ${CMAKE_SOURCE_DIR}/proto/memory.proto)

# 3) Directorio donde se generarán los .pb/.grpc.*.cc/h
set(GENERATED_DIR ${CMAKE_SOURCE_DIR}/proto)
file(MAKE_DIRECTORY ${GENERATED_DIR})

# 4) Rutas a protoc y plugin gRPC
set(_PROTOBUF_PROTOC         ${Protobuf_PROTOC_EXECUTABLE})
# Suponiendo que vcpkg está en C:/dev/vcpkg
set(_GRPC_CPP_PLUGIN "/Users/santirobles/vcpkg/installed/arm64-osx/tools/grpc/grpc_cpp_plugin")

#set(_GRPC_CPP_PLUGIN "E:/ClionProjects/vcpkg/installed/x64-windows/tools/grpc/grpc_cpp_plugin.exe") #"E:/ClionProjects/vcpkg/installed/x64-windows/tools/grpc/grpc_cpp_plugin.exe" //


set(_GRPC_CPP_PLUGIN_OPTIONS "--plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN}")

# 5) Definimos archivos de salida
get_filename_component(proto_name ${PROTO_FILE} NAME_WE)
set(pb_src   "${GENERATED_DIR}/${proto_name}.pb.cc")
set(pb_hdr   "${GENERATED_DIR}/${proto_name}.pb.h")
set(grpc_src "${GENERATED_DIR}/${proto_name}.grpc.pb.cc")
set(grpc_hdr "${GENERATED_DIR}/${proto_name}.grpc.pb.h")

# 6) Comando para invocar protoc y generar .pb.cc/.pb.h y .grpc.pb.cc/.grpc.pb.h
add_custom_command(
        OUTPUT "${pb_src}" "${pb_hdr}" "${grpc_src}" "${grpc_hdr}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --proto_path=${CMAKE_SOURCE_DIR}/proto
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        ${_GRPC_CPP_PLUGIN_OPTIONS}
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        COMMENT "Generating Protobuf & gRPC files from ${PROTO_FILE}"
        VERBATIM
)



# 7) Creamos una librería con los archivos generados
add_library(memory_proto
        ${pb_src}
        ${grpc_src}
)
# Asegúrate de incluir la carpeta generada, para encontrar los .h
target_include_directories(memory_proto PUBLIC ${GENERATED_DIR})
target_link_libraries(memory_proto
        PUBLIC
        gRPC::grpc++
        protobuf::libprotobuf
)

# 8) Ejecutable del servidor
add_executable(server src/memory_manager.cpp)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(server PRIVATE memory_proto)

# 9) Ejecutable del cliente
add_executable(client src/client.cpp)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(client PRIVATE memory_proto)


# Crear la biblioteca del Mock Memory Manager
add_library(mock_memory_manager src/mock_memory_manager.cpp)

# Agregar el ejecutable para pruebas
add_executable(test_mpointers test/test_mpointers.cpp)
target_link_libraries(test_mpointers mock_memory_manager)