#include <iostream>
#include "../include/mpointer.h"  // <-- Ajusta la ruta según tu proyecto

// ============================================================================
//                         Estructura Node<T>
// ============================================================================

template <typename T>
struct Node {
    T data;                  // Dato del nodo
    MPointer<Node<T>> next;  // MPointer al siguiente nodo

    Node() : data(), next() {}
    Node(const T& val) : data(val), next() {}
};

// ============================================================================
//              Clase MList<T>: Lista enlazada con MPointer<Node<T>>
// ============================================================================

template <typename T>
class MList {
private:
    MPointer<Node<T>> head;  // La cabeza de la lista

public:
    // Constructor: lista inicialmente vacía
    MList() : head() {
        // Si head es "cero" (o un id nulo), significa lista vacía
    }

    // Inserta un valor al frente de la lista
    void push_front(const T& value) {
        // 1. Creamos un nodo remoto
        MPointer<Node<T>> newNode = MPointer<Node<T>>::New();

        // 2. Preparar un Node<T> local que llenaremos con 'value' y el enlace a 'head'

        Node<T> temp = *newNode;
        temp.data = value;
        temp.next = head;

        // 3. Guardamos este Node<T> en el objeto remoto apuntado por newNode
        *newNode = temp;

        // 4. Actualizamos la cabeza de la lista
        head = newNode;
    }

    // Recorre la lista e imprime los valores
    void printList() const {
        // Empezamos desde la cabeza
        MPointer<Node<T>> current = head;

        // Mientras current no sea nulo
        while (true) {
            if (current.id == 0) {
                break;
            }

            // Obtenemos localmente el contenido del nodo remoto
            Node<T> nodeData = *current;
            std::cout << nodeData.data << std::endl;

            // Avanzamos al siguiente
            current = nodeData.next;
        }
    }

    // Comprueba si un MPointer<Node<T>> es nulo
    // En tu implementación, &mpointer retorna el ID.
    // Asumimos que un id <= 0 indica que está "vacío" o "nulo".
    bool isNull(const MPointer<Node<T>>& ptr) const {
        return (&ptr) <= 0;
    }
};

// ============================================================================
//                               main()
// ============================================================================

int main() {
    // 1. Inicializar el sistema de MPointers (si no se hace en otro lado)
    //    Esto llamará internamente a globalClient->initiateClient(), etc.
    MPointer<int>::Init();

    // 2. Creamos una lista de enteros
    MList<int> myList;

    // 3. Insertamos algunos valores al frente

    myList.push_front(10);
    myList.push_front(20);
    myList.push_front(30);

    // 4. Imprimimos la lista (debería mostrar 30, 20, 10)
    //std::cout << "Contenido de la lista enlazada (inicio -> fin):" << std::endl;
    myList.printList();

    return 0;
}