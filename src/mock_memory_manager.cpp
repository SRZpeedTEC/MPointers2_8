#include "../include/mock_memory_manager.h"

int MockMemoryManager::Create(size_t size) {
    int id = NextId++;  // Genera un nuevo ID único
    memory[id] = 0;  // Inicializa en 0 (simula reservar memoria)

    std::cout << "MockMemoryManager: Se ha creado un bloque con ID " << id
              << " y tamaño " << size << " bytes." << std::endl;

    return id;  // Retorna el ID para que MPointer<T> lo use
}

void MockMemoryManager::Set(int id, int value) {
    if (memory.find(id) != memory.end()) {  // Verifica si el ID existe
        memory[id] = value;  // Almacena el valor en la memoria simulada

        std::cout << "MockMemoryManager: Se ha almacenado el valor " << value
                  << " en el bloque con ID " << id << "." << std::endl;
    } else {
        std::cerr << "MockMemoryManager: Error - Intento de escribir en un bloque inexistente (ID "
                  << id << ")." << std::endl;
    }
}

int MockMemoryManager::Get(int id) {
    if (memory.find(id) != memory.end()) {  // Verifica si el ID existe
        std::cout << memory[id] << std::endl;
        return memory[id];  // Devuelve el valor almacenado
    } else {
        std::cerr << "MockMemoryManager: Error - Intento de leer un bloque inexistente (ID "
                  << id << ")." << std::endl;
        return -1;  // Devuelve un valor de error
    }
}
void MockMemoryManager::IncreaseRefCount(int id) {
    if (memory.find(id) != memory.end()) {  // Verifica si el ID existe
        std::cout << "MockMemoryManager: Incrementando contador de referencias del bloque con ID "
                  << id << "." << std::endl;
    } else {
        std::cerr << "MockMemoryManager: Error - Intento de incrementar referencia de un bloque inexistente (ID "
                  << id << ")." << std::endl;
    }
}

void MockMemoryManager::DecreaseRefCount(int id) {
    if (memory.find(id) != memory.end()) {  // Verifica si el ID existe
        std::cout << "MockMemoryManager: Decrementando contador de referencias del bloque con ID "
                  << id << "." << std::endl;
    } else {
        std::cerr << "MockMemoryManager: Error - Intento de decrementar referencia de un bloque inexistente (ID "
                  << id << ")." << std::endl;
    }
}