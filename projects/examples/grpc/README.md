# Overview

An example project that demonstrates how to set up a gRPC server and client in C++.

# Getting Started

1. Compile the proto IDL to generate C++ code

```bash
bazel build projects/examples/grpc/proto:example_cc_grpc_compile
```

2. Modify C++ server and client code leveraging the generated header `example.grpc.pb.h`

3. Build server and client programs

```bash
# To build the example gRPC server only
bazel build projects/examples/grpc:example_grpc_server

# To build the example gRPC client only
bazel build projects/examples/grpc:example_grpc_client

# Build everything
bazel build projects/examples/grpc/...
```

4. Run both programs in 2 separate shell sessions

```bash
# Run the server program
bazel-bin/projects/examples/grpc/example_grpc_server
```

Alternatively, you can build and run in 1 command

```bash
bazel run projects/examples/grpc:example_grpc_server
```

# Usage

## Server and Client Automatically Interacting With Each Other

Out-of-the-box, the example gRPC client repeatedly sends a request to the example server at a periodic interval. Let's try this out ourselves.

1. In one shell session, run the gRPC server

```bash
bazel run projects/examples/grpc:example_grpc_server
# ...
# Server listening on localhost:30000
# Received request: "Test message (counter: 0)"
```

2. In another shell session, run the gRPC client

```bash
bazel run projects/examples/grpc:example_grpc_client
# ...
# Received response: "Test message (counter: 0)" (Status Code: 200)
```

3. Observe how the gRPC client connects to the server then repeatedly calls a method

## Manually Testing The Server With a Command Line gRPC Client

In this exercise, we will learn how to interact with a gRPC server using `grpcurl`, a command line gRPC client. This is made possible thanks to [gRPC reflections](https://grpc.io/docs/guides/reflection/), a plugin that adds debug information embedded inside your gRPC program which includes stuff about the available services, methods, description of the methods and its fields, of course at the cost of increasing your application file sizes.

1. First run the gRPC server. In this case, it be listening on `localhost:30000`

```bash
bazel run projects/examples/grpc:example_grpc_server
```

2. Inspect the server's services

```bash
# List all available services
grpcurl -plaintext localhost:30000 list

# List information about a specific service
grpcurl -plaintext localhost:30000 list com.example.grpc.RequestService

# List information about a specific method
grpcurl -plaintext localhost:30000 describe com.example.grpc.RequestService.Send
## com.example.grpc.RequestService.Send is a method:
## rpc Send ( .com.example.grpc.Request ) returns ( .com.example.grpc.Response );
```

3. Let's invoke a method via command line. In this example, it takes 1 input (message) and produces 2 outputs (HTTP status code, echoed back message)

```bash
grpcurl -plaintext -d '{"msg": "Hello world!"}' localhost:30000 com.example.grpc.RequestService.Send
# {
#   "statusCode": 200,
#   "echoMsg": "Hello world!"
# }
```
