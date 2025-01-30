#!/usr/bin/env python3
# References:
# - https://grpc.io/docs/languages/python/basics/#client
import sys
from argparse import ArgumentParser, RawTextHelpFormatter
from pathlib import Path
import time

import grpc
import projects.examples.grpc.proto.example_pb2 as example_pb2
import projects.examples.grpc.proto.example_pb2_grpc as example_pb2_grpc

DESCRIPTION = """\
Example gRPC client
This program connects to the example gRPC server and periodically calls a method indefinitely

Example usage:
./{filename}
""".format(
    filename=Path(__file__).name
)


class ExampleGrpcClient:
    """
    Responsible for acting as an example gRPC client that simply sends a request containing any message on demand to the example request service
    """

    def __init__(self, addr: str, port: int, timeout: float = 5.0):
        """
        :param addr: Server address to connect to on a method call
        :param port: Server port to connect to on a method call
        :param timeout: Transaction timeout on a method call in seconds
        """
        self._addr = addr
        self._port = port
        self._timeout = timeout

        channel: grpc.Channel = grpc.insecure_channel(f"{addr}:{port}")
        self._request_service_stub = example_pb2_grpc.RequestServiceStub(channel)

    def send_request(self, message: str) -> bool:
        """
        Transact a request
        :param message: Any human readable message
        :return: True if the message was able to connect and transact a request/response
        """
        success = False
        request = example_pb2.Request(msg=message)

        try:
            response: example_pb2.Response = self._request_service_stub.Send(
                request,
                timeout=self._timeout,
            )
            print(f"Received response:\n{response}")
            success = True
        except grpc.RpcError as err:
            print(
                f"Unable to send to {self._addr}:{self._port} within {self._timeout} s. Error: {err}"
            )

        return success

    @property
    def timeout(self) -> float:
        return self._timeout

    @timeout.setter
    def timeout(self, new_timeout):
        self._timeout = new_timeout


def get_args():
    argument_parser = ArgumentParser(
        description=DESCRIPTION, formatter_class=RawTextHelpFormatter
    )
    argument_parser.add_argument(
        "-a",
        "--addr",
        default="localhost",
        help="Connect address of gRPC server",
    )
    argument_parser.add_argument(
        "-p",
        "--port",
        type=int,
        default=30000,
        help="Connect port of gRPC server",
    )
    return argument_parser.parse_args()


def main():
    exit_code = 1
    args = get_args()

    gprc_client = ExampleGrpcClient(
        addr=args.addr,
        port=args.port,
    )

    try:
        counter = 0
        while True:
            message = f"Test message (counter: {counter})"
            counter += 1
            gprc_client.send_request(message)
            time.sleep(3)
    except KeyboardInterrupt:
        exit_code = 0

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
