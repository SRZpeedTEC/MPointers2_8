#include "../include/mpointer.h"
#include <iostream>

int main() {
    std::cout << "\n=== INICIANDO PRUEBAS DE MPOINTER<T> ===\n" << std::endl;

    // Aquí se inicializa la biblioteca de MPointers
    MPointer<string>::Init();
    std::cout << "Cliente iniciado, conexión establecida " << std::endl;


    //Crear un mpointer
    MPointer<string> p1 = MPointer<string>::New();
    std::cout << "MPointer<int> creado con ID: " << &p1 << std::endl;

    // hacerle un set al mpointer
    *p1 = "Hola";
    string str = *p1;
    std::cout << "Valor almacenado en p1: " << str << std::endl;

    // crear otro mpointer y hacerle un set
    MPointer<string> p2 = MPointer<string>::New();
    *p2 = "Mundo";
    p2 = "HOLA";
    string str2  = *p2;
    std::cout << "MPointer<int> creado con ID: " << &p2 << std::endl;
    std::cout << "Valor almacenado en p2: " << str2 << std::endl;

    // crear un tercer mpointer y que apunte a lo mismo que apunta p1
    MPointer<string> p3 = MPointer<string>::New();
    p3 = p2;
    string str3 = *p3;
    std::cout << "p3 ahora apunta al mismo bloque que p2 con ID: " << &p2 << std::endl;
    std::cout << "p3 ahora apunta al mismo bloque que p2 con ID: " << &p3 << std::endl;

    // ver que efectivamente apuntan al mismo lado
    std::cout << "Valor de p3 (debe ser el mismo que p2): " << str3 << std::endl;
    std::cout << "Valor de p2  " << str2 << std::endl;

    // destruir los punteros ** todavia tengo que ver si funciona bien
    {
        MPointer<int> p4 = MPointer<int>::New();
        *p4 = 90;
        int str4  = *p4;
        std::cout << "MPointer<int> p4 creado y almacenando: " << str4 << std::endl;
    }  // Aquí `p4` sale del scope y su destructor se ejecuta

    std::cout << "\n=== PRUEBAS FINALIZADAS ===\n" << std::endl;



    return 0;


}