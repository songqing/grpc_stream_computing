// file stream_computing_server.cc
#include "stream_computing.grpc.pb.h"
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>

#include <time.h>

using streamcomputing::StreamComputing;
using streamcomputing::Object;
using streamcomputing::NullMessage;
class StreamComputingImpl final : public StreamComputing::Service
{
public:
  clock_t prev;
    StreamComputingImpl() {
      prev = clock();
    }
    grpc::Status PutObject(grpc::ServerContext *context, grpc::ServerReader<Object> *request, 
                             NullMessage *response) override
    {
        Object object;
        while (request->Read(&object)) {
          int32_t obj=  object.obj();
          if (obj % 10000 == 1) {
            clock_t now = clock();
            std::cout << "get obj: " << obj << ": " << (double)(now-prev)/CLOCKS_PER_SEC << std::endl;
            prev=now;
          }
        }
        return grpc::Status::OK;
    }
    
};
int main(int argc, char *argv[])
{
    std::string addr = "0.0.0.0:5000";
    StreamComputingImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server = builder.BuildAndStart();
    std::cout << "Server listening on " << addr << std::endl;
    server->Wait();
    return 0;
}
