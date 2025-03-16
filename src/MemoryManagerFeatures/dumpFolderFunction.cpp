#include "dumpFolderFunction.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include<map>
#include <filesystem>

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
    filesystem::create_directories(folderPath);
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
    strftime(timeBuf, sizeof(timeBuf), "%H-%M-%S", &tmLocal);

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

    out << left
    << setw(4)  << "ID"
    << setw(8)  << "Size"
    << setw(90) << "Type"
    << setw(30)  << "RefCount"
    << setw(30) << "start_ptr"
    << endl;

    // Luego cada fila
    for (auto &Block : memoryBlocksMap) {
        int blockID = Block.first;
        const memoryBlockInfo &blockInfo = Block.second;
        void* ptrVal = blockInfo.start_ptr;

        // Imprimir fila con la misma alineación y anchuras
        out << left
            << setw(4)  << blockID
            << setw(8)  << blockInfo.size
            << setw(90) << blockInfo.type
            << setw(30)  << blockInfo.refcount
            << setw(30) << ptrVal
            << endl;
    }
    out.close();

    cout << "[dumpFolderUpdate] Se creo el archivo: " << filename.str() << endl;

}

