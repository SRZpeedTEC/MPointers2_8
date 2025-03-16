#pragma once
#ifndef MEMORYBLOCKINFO_H
#define MEMORYBLOCKINFO_H

#include <string>
#include <cstddef>  // Para size_t

using namespace std;

struct memoryBlockInfo  // Creacion de una estructura para representar los bloques de memoria
{
    void* start_ptr;
    size_t size;
    string type;
    int refcount;
};

#endif // MEMORYBLOCKINFO_H