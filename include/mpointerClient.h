//
// Created by XPC on 3/15/2025.
//

#ifndef MPOINTERCLIENT_H
#define MPOINTERCLIENT_H


using namespace std;



class mpointerClient
{
public:

    void initiateClient();
    int HandleCreateRequest(string Type, size_t size);
    bool HandleSetRequest(int id, string binValue);
    string HandleGetRequest(int id);
    bool HandleIncreaseRefCountRequest(int id);
    bool HandleDecreaseRefCountRequest(int id);



private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<memmgr::MemoryManager::Stub> stub;

};


#endif //MPOINTERCLIENT_H
