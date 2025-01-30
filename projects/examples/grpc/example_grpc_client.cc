/**
 * Example gRPC client
 * This program connects to the example gRPC server and periodically calls a method indefinitely
 * References:
 * - https://grpc.io/docs/languages/cpp/basics/#client
 * - https://github.com/grpc/grpc/tree/master/examples/cpp/error_handling
 * - https://grpc.io/blog/deadlines/#c
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "grpcpp/grpcpp.h"
#include "projects/examples/grpc/proto/example.grpc.pb.h"

/**
 * Responsible for acting as an example gRPC client that simply sends a request containing any message on demand
 * to the example request service
 */
class ExampleGrpcClient {
 public:
  /**
   * @param[in] addr: Server address to connect to on a method call
   * @param[in] port: Server port to connect to on a method call
   * @param[in] timeout: Transaction timeout on a method call
   */
  ExampleGrpcClient(const std::string& addr, uint16_t port, std::chrono::milliseconds timeout = std::chrono::milliseconds(5000)) : timeout_(timeout) {
    const std::string target = addr + ":" + std::to_string(port);
    const std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    request_service_stub_ = com::example::grpc::RequestService::NewStub(channel);
  }

  /**
   * Transact a request for a response
   * @param[in] message: Any human readable message
   * @return true if the message was able to connect and transact a request/response
   */
  bool send_request(const std::string& message) {
    bool sent = false;

    grpc::ClientContext context;
    com::example::grpc::Request request;
    com::example::grpc::Response response;

    context.set_deadline(std::chrono::system_clock::now() + timeout_);

    request.set_msg(message);
    grpc::Status status = request_service_stub_->Send(&context, request, &response);

    if (status.ok()) {
      sent = true;
      std::cout << "Received response: \"" << response.echo_msg() << "\" (Status Code: " << response.status_code() << ")" << std::endl;
    } else {
      std::cout << "Failed to send request after timeout (" << timeout_.count() << " ms)" << " Error: " << status.error_message() << std::endl;
    }

    return sent;
  }

 private:
  std::chrono::milliseconds timeout_;
  std::unique_ptr<com::example::grpc::RequestService::Stub> request_service_stub_;
};

ABSL_FLAG(std::string, addr, "localhost", "Connect address of gRPC server");
ABSL_FLAG(uint16_t, port, 30000, "Connect port of gRPC server");

int main(int argc, char* argv[]) {
  int exit_code = EXIT_FAILURE;

  absl::ParseCommandLine(argc, argv);

  ExampleGrpcClient client(absl::GetFlag(FLAGS_addr), absl::GetFlag(FLAGS_port));

  uint8_t message_counter = 0U;
  while (true) {
    const std::string message = "Test message (counter: " + std::to_string(message_counter) + ")";
    ++message_counter;
    (void)client.send_request(message);
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

  exit_code = EXIT_SUCCESS;

  return exit_code;
}
