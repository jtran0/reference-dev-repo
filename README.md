# Overview

This repo serves as a quick jumpstart baseline to develop up your own C++ projects. Out-of-the-box this repo an assortment of premade infrastructure such as:

**Docker**: Included is a development container bundled with an assortment of developer software such a C/C++ toolchains, Bazel, Python, etc.

**Bazel**: Included is Bazel and configurations to compile C++ executables and Protobuf/gRPC.

**Example C++ Code**: Included is an example C++ project that demonstrates how to build your own gRPC server and client.

**Extra Developer Tools**: Included are quality-of-life developer tools such as Clang auto code formatter.

This repo is ready to be used as-is. Start off today by cloning this repo, then follow the [Getting Started](#getting-started) workflow to build one of the example projects.

# Getting Started

This getting started workflow is intended to help you get started from scratch and to teach you how to use this repo. After cloning this repo, follow this workflow from start to finish. By the end of this section, you should have achieved the following:

- You have VSCode and Docker Engine installed locally
- You have the repo's developer Docker container running in the background and ready to use
- You are able to switch to `Container Mode` in VSCode
- You are able to build and run C++ programs from one of the example projects

## Step 1. Ensure Prerequisites

This repo is intended to be used on Linux environments and is tested on Ubuntu 24.04 LTS. Older Ubuntu versions such as 22.04 LTS and 20.04 LTS should also work.

1. Ensure your system meets the following criteria:

    - Linux distribution (Recommended: Ubuntu 24.04 LTS)
    - x86-64 CPU architecture

This project comes packaged with a Docker image intended to act as your development environment. The container is intended to be managed by [Visual Studio Code](https://code.visualstudio.com/). All Docker related files resides in the `.devcontainer` directory.

2. Ensure you have VSCode installed and the neccessary extensions:

    - [VSCode](https://code.visualstudio.com/)
    - [VSCode Extension: Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) (Install using VSCode GUI)

The development Docker container runs indefinitely in the background and serves as your development environment ready to be used at any time. It contains development software tools such as Bazel, native toolchain, Python, etc.

3. Ensure [Docker Engine](https://docs.docker.com/engine/) is installed.

    **RUN THIS SETUP SCRIPT** to install Docker and to set up other related prerequisites.

```bash
./setup.sh
```

## Step 2. Switch to Container Mode

As a background, I usually operate remotely where my source code and personal shell customizations all reside on a remote Linux server. In this case, my VSCode would be in `SSH Mode`. Similarly, `Local Mode` is when the source code and shell customizations all reside locally. This repo is intended to operate in `Container Mode` which is a standardized development environment (shell customizations, command line utilities, etc.) controlled by the contents of the `.devcontainer` directory and the Docker image residing in it.

1. Assuming this is your first time using this repo, observe that you are in `Local Mode` or `SSH Mode`. For example, I started off in SSH mode.

![SSH Mode](docs/assets/ssh_mode.png)

2. Using the [Command Palette](https://docs.github.com/en/codespaces/reference/using-the-vs-code-command-palette-in-codespaces#accessing-the-vs-code-command-palette), rebuild and reopen to `Container Mode`. This step may take a couple of minutes as the Docker image builds in the background.

![Switch Container Mode](docs/assets/switch_container_mode.png)

3. Observe that you are in `Container Mode`. In this mode, you can [Open Terminal](https://code.visualstudio.com/docs/terminal/basics) which will open a shell session inside the development Docker container.

![Container Mode](docs/assets/container_mode.png)

At this point, you should be good to proceed to the next step. As a side note, you can revert back to `Local/SSH Mode` at any time using the [Command Palette](https://docs.github.com/en/codespaces/reference/using-the-vs-code-command-palette-in-codespaces#accessing-the-vs-code-command-palette).

![Switch SSH Mode](docs/assets/switch_ssh_mode.png)

## Step 3. Build Your First Hello World Program

To make sure everything is work, let's test your setup by building the hello world program.

1. Ensure you are in `Container Mode`

2. Build the example hello world C++ program

```bash
bazel build projects/examples/hello_world
```

3. Run the hello world program

```bash
bazel-bin/projects/examples/hello_world/hello_world
# Hello world!
```

# Usage

To use this project, **YOU MUST USE THE DEVELOPMENT CONTAINER**. Do not skip the [Getting Started](#getting-started) workflow.

## Build

To build a C++ project, in `Container Mode`, build any Bazel project using the `bazel` command.

```bash
# For example, build every project
bazel build projects/...
```

**Not Recommended:** Alternatively, you can build while in `Local/SSH Mode`, but you must use the development container.

```bash
docker exec -it devcontainer bazel build projects/...
```

# Troubleshooting

### Docker container `devcontainer` already exists

Switching to `Container Mode` may fail because the Docker image build may fail due to the container `devcontainer` already exists. In this case, simply delete the container, rebuild the image, and restart the container.

1. While in `Local/SSH Mode`, stop and delete the container:

```bash
docker container rm -f devcontainer
```

2. Reattempt to switch to `Container Mode`.

# For Maintainers

This section is intended for maintainers only.

## Docker

To edit and test the development Docker container:

1. Edit Docker related files inside the `.devcontainer` directory.

2. In VSCode command palette, run:

![Rebuild Container](docs/assets/rebuild_container.png)

3. Test Docker changes either in `Container Mode` OR manually using the `docker exec` command.

```bash
# Alternatively, manually open a shell session in the development container
docker exec -it devcontainer bash
```

## VSCode Extensions

To add/remove the standardized list of VSCode extensions:

1. Edit the list of extensions inside the Dev Container configuration file `.devcontainer/devcontainer.json`
