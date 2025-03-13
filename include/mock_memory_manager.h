//
// Created by Santi Robles on 12/3/25.
//

#ifndef MOCK_MEMORY_MANAGER_H
#define MOCK_MEMORY_MANAGER_H

#include <unordered_map>
#include <iostream>
#include <cstddef>

using namespace std;  // Evita escribir std:: antes de size_t

class MockMemoryManager {
  private:
    std::unordered_map<int, int> memory;
    int NextId = 1;

  public:
    int Create(size_t size);
    void Set(int id, int value);
    int Get(int id);
    void IncreaseRefCount(int id);
    void DecreaseRefCount(int id);

};


#endif //MOCK_MEMORY_MANAGER_H
