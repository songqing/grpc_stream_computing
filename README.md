# grpc stream computing

Using grpc/stream rpc to do a simple stream computing

steps of compiling:
1 
protoc -I protos --grpc_out=cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/stream_computing.proto
protoc -I protos --cpp_out=cpp protos/stream_computing.proto

2
clang++ -std=c++11 -o server -lgrpc++ -lprotobuf -lpthread -lgrpc++_reflection stream_computing.pb.cc stream_computing.grpc.pb.cc stream_computing_server.cc
clang++ -std=c++11 -o client -lgrpc++ -lprotobuf -lpthread -lgrpc++_reflection stream_computing.pb.cc stream_computing.grpc.pb.cc stream_computing_client.cc

