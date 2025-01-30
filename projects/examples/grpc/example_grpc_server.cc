/**
 * Example gRPC server
 * References:
 * - https://grpc.io/docs/languages/cpp/basics/#defining-the-service
 * - https://github.com/grpc/grpc/tree/master/examples/cpp/error_handling
 * - https://github.com/grpc/grpc/tree/master/examples/cpp/reflection
 */

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "grpcpp/ext/proto_server_reflection_plugin.h"
#include "grpcpp/grpcpp.h"
#include "projects/examples/grpc/proto/example.grpc.pb.h"

/**
 * GRPC request service implementation
 */
class RequestServiceImpl final : public com::example::grpc::RequestService::Service {
  grpc::Status Send(grpc::ServerContext *context, const com::example::grpc::Request *request, com::example::grpc::Response *response) override {
    std::cout << "Received request: \"" << request->msg() << "\"" << std::endl;

    constexpr int http_status_code_success = 200;
    response->set_status_code(http_status_code_success);
    response->set_echo_msg(request->msg());

    return grpc::Status::OK;
  }
};

/**
 * Responsible for acting as a GRPC server that handles the request service
 */
class ExampleGrpcServer {
 public:
  /**
   * @param[in] addr: Listen address to bind
   * @param[in] port: Listen port to bind
   */
  ExampleGrpcServer(const std::string &addr, uint16_t port) : addr_(addr), port_(port) {}

  /**
   * Start the server by binding to the listening addr:port then block indefinitely
   * @return false if the server failed to start. In case of failure, the error logs are printed to stderr
   */
  bool start_blocking() {
    bool started = false;
    const std::string addr_uri = addr_ + ":" + std::to_string(port_);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr_uri, grpc::InsecureServerCredentials());

    RequestServiceImpl service;
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (server) {
      started = true;
      std::cout << "Server listening on " << addr_uri << std::endl;
      server->Wait();
    }

    return started;
  }

 private:
  std::string addr_;
  uint16_t port_;
};

ABSL_FLAG(std::string, addr, "localhost", "gRPC server listen address");
ABSL_FLAG(uint16_t, port, 30000, "gRPC server listen port");

int main(int argc, char *argv[]) {
  int exit_code = EXIT_FAILURE;

  absl::ParseCommandLine(argc, argv);

  grpc::reflection::InitProtoReflectionServerBuilderPlugin();  // Enable gRPC reflection for debugging purposes
  ExampleGrpcServer example_grpc_service(absl::GetFlag(FLAGS_addr), absl::GetFlag(FLAGS_port));
  const bool started = example_grpc_service.start_blocking();

  if (started) {
    exit_code = EXIT_SUCCESS;
  }

  return exit_code;
}
