
#ifndef DUMPFOLDERFUNCTION_H
#define DUMPFOLDERFUNCTION_H
#include <string>
#include <map>
#include "../MemoryManager/memoryBlockInfo.h"

using namespace std;


class dumpFolderFunction {

public:
    dumpFolderFunction(const string& folderPath);
    ~dumpFolderFunction();

    void dumpFolderUpdate(map<int, memoryBlockInfo>& memoryBlocks);

private:
    const string& folderPath;
};



#endif //DUMPFOLDERFUNCTION_H
