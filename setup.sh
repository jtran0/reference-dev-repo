#!/usr/bin/env bash
set -eo pipefail

sudo echo "" > /dev/null

# Install docker
## https://docs.docker.com/engine/install/ubuntu/
## Conditionally install docker only if it isn't installed yet
if ! docker run hello-world &> /dev/null; then
  sudo apt install -y \
      ca-certificates \
      curl \
      gnupg \
      lsb-release

  sudo mkdir -p /etc/apt/keyrings
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor --yes -o /etc/apt/keyrings/docker.gpg

  echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

  sudo chmod a+r /etc/apt/keyrings/docker.gpg
  sudo apt update
  sudo apt install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin

  # Add the current user to the docker group
  # https://docs.docker.com/engine/install/linux-postinstall/
  sudo groupadd docker
  sudo usermod -aG docker $USER
  newgrp docker
fi

# Install Bazel
## https://bazel.build/versions/8.0.0/install/ubuntu#docker-container
## Pre install setup
sudo mkdir -p ${HOME}/.cache/bazel
sudo chown $(id -u):$(id -g) ${HOME}/.cache/bazel
## At this point, VSCode extension `devcontainer` is responsible for building a development Docker container which contains Bazel
