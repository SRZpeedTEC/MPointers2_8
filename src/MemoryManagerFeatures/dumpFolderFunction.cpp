#include "dumpFolderFunction.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include<map>

using namespace std::chrono;


dumpFolderFunction::dumpFolderFunction(const string& folder_Path)
    : folderPath(folder_Path)
{

}

dumpFolderFunction::~dumpFolderFunction()
{

}

void dumpFolderFunction::dumpFolderUpdate(map<int, memoryBlockInfo>& memoryBlocksMap)
{
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    time_t t = system_clock::to_time_t(now);

    std::tm tmLocal;
#ifdef _WIN32
    localtime_s(&tmLocal, &t);
#else
    localtime_r(&t, &tmLocal);
#endif

    char timeBuf[64];
    strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &tmLocal);

    ostringstream filename;
    filename << folderPath << "/dump_"
            << timeBuf << "_"
            << setw(3) << setfill('0') << ms.count()
    << ".txt";

    ofstream out(filename.str());
    if (!out.is_open())
    {
        std::cerr << "No se pudo crear el archivo de dump: " << filename.str() << std::endl;
        return;
    }

    out << "ID\tSize\tType\tRefCount\tstart_ptr\n";

    for (auto &Block : memoryBlocksMap)
    {
        int blockID = Block.first;
        const memoryBlockInfo &blockInfo = Block.second;

        void* ptrVal = static_cast<void*>(blockInfo.start_ptr);

        out << blockID << "\t"
            << blockInfo.size << "\t"
            << blockInfo.type << "\t"
            << blockInfo.refcount << "\t"
            << ptrVal << "\n";
    }
    out.close();

    cout << "[dumpFolderUpdate] Se creo el archivo: " << filename.str() << endl;

}

