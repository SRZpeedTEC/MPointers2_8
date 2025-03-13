#include "../include/mpointer.h"
#include <iostream>

int main() {
    std::cout << "\n=== INICIANDO PRUEBAS DE MPOINTER<T> ===\n" << std::endl;

    // Aquí se inicializa la biblioteca de MPointers
    MPointer<int>::Init();

    //Crear un mpointer
    MPointer<int> p1 = MPointer<int>::New();
    std::cout << "MPointer<int> creado con ID: " << &p1 << std::endl;

    // hacerle un set al mpointer
    *p1 = 42;
    std::cout << "Valor almacenado en p1: " << *p1 << std::endl;

    // crear otro mpointer y hacerle un set
    MPointer<int> p2 = MPointer<int>::New();
    *p2 = 99;
    std::cout << "MPointer<int> creado con ID: " << &p2 << std::endl;
    std::cout << "Valor almacenado en p2: " << *p2 << std::endl;

    // crear un tercer mpointer y que apunte a lo mismo que apunta p1
    MPointer<int> p3 = MPointer<int>::New();
    p3 = p1;
    std::cout << "p3 ahora apunta al mismo bloque que p1 con ID: " << &p3 << std::endl;

    // ver que efectivamente apuntan al mismo lado
    std::cout << "Valor de p3 (debe ser el mismo que p1): " << *p3 << std::endl;

    // destruir los punteros ** todavia tengo que ver si funciona bien
    {
        MPointer<int> p4 = MPointer<int>::New();
        *p4 = 77;
        std::cout << "MPointer<int> p4 creado y almacenando: " << *p4 << std::endl;
    }  // Aquí `p4` sale del scope y su destructor se ejecuta

    std::cout << "\n=== PRUEBAS FINALIZADAS ===\n" << std::endl;
    return 0;
}