#include "../include/mpointer.h"
#include <iostream>

int main() {
    std::cout << "\n=== INICIANDO PRUEBAS DE MPOINTER<T> ===\n" << std::endl;

    // Inicializar el manejador de memoria
    MPointer<int>::Init();

    // 1️⃣ PRUEBA: Crear un MPointer<int>
    MPointer<int> p1 = MPointer<int>::New();
    std::cout << "MPointer<int> creado con ID: " << &p1 << std::endl;

    // 2️⃣ PRUEBA: Asignar un valor al MPointer<int>
    *p1 = 42;
    std::cout << "Valor almacenado en p1: " << *p1 << std::endl;

    // 3️⃣ PRUEBA: Crear otro MPointer<int> y asignarle un valor
    MPointer<int> p2 = MPointer<int>::New();
    *p2 = 99;
    std::cout << "MPointer<int> creado con ID: " << &p2 << std::endl;
    std::cout << "Valor almacenado en p2: " << *p2 << std::endl;

    // 4️⃣ PRUEBA: Copiar referencia de p1 a p3
    MPointer<int> p3 = MPointer<int>::New();
    p3 = p1;  // p3 ahora apunta al mismo ID que p1
    std::cout << "p3 ahora apunta al mismo bloque que p1 con ID: " << &p3 << std::endl;

    // 5️⃣ PRUEBA: Verificar que p3 obtiene el mismo valor que p1
    std::cout << "Valor de p3 (debe ser el mismo que p1): " << *p3 << std::endl;

    // 6️⃣ PRUEBA: Destrucción de punteros y manejo de referencias
    {
        MPointer<int> p4 = MPointer<int>::New();
        *p4 = 77;
        std::cout << "MPointer<int> p4 creado y almacenando: " << *p4 << std::endl;
    }  // Aquí `p4` sale del scope y su destructor se ejecuta

    std::cout << "\n=== PRUEBAS FINALIZADAS ===\n" << std::endl;
    return 0;
}