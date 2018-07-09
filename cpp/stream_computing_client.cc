#include "stream_computing.grpc.pb.h"
#include <iostream>
#include <memory>
#include <string>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
using streamcomputing::StreamComputing;
using streamcomputing::Object;
using streamcomputing::NullMessage;
class StreamComputingClient
{
public:
    StreamComputingClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(StreamComputing::NewStub(channel))  { }
    void PutObject()
    {
        grpc::ClientContext context;
        NullMessage stats;
        Object object;

        std::unique_ptr<grpc::ClientWriter<Object> > writer(stub_->PutObject(&context, &stats));

        for (int i = 0; i< 100000; ++i) {
          object.set_obj(i);
          if (!writer->Write(object)) {
            std::cout << "error occured" << std::endl;
            break;
          }
        }
        writer->WritesDone();
        grpc::Status status = writer->Finish();
        if (status.ok()) {
          std::cout << "put success" << std::endl;
        } else {
          std::cout << "put failed" << std::endl;
        }
    }
    
private:
    std::unique_ptr<StreamComputing::Stub> stub_;
};
int main(int argc, char *argv[])
{
    auto channel = grpc::CreateChannel("localhost:5000", grpc::InsecureChannelCredentials());
    StreamComputingClient client(channel);
    client.PutObject();
    return 0;
}
