FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install APT dependencies
COPY requirements-apt.txt /tmp/requirements-apt.txt
RUN apt-get update && grep -vE '^\s*#|^\s*$' /tmp/requirements-apt.txt | xargs apt-get install -y --no-install-recommends

# Install Python dependencies
COPY requirements-pip.txt /tmp/requirements-pip.txt
RUN pip install --no-cache-dir --break-system-packages -r /tmp/requirements-pip.txt

# Install Bazel
## https://bazel.build/install/ubuntu
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor -o /usr/share/keyrings/bazel-archive-keyring.gpg && \
  echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
  apt-get update && apt-get install -y \
  bazel-7.4.1 \
  && \
  ln -s /usr/bin/bazel-7.4.1 /usr/bin/bazel

# Install Bazel Buildifier
## https://github.com/bazelbuild/buildtools/releases
RUN curl -Lo /usr/local/bin/buildifier https://github.com/bazelbuild/buildtools/releases/download/v8.0.1/buildozer-linux-amd64 && \
  chmod +x /usr/local/bin/buildifier

# Set up Unix user/group
## Replace uid=1000(ubuntu) and gid=1000(ubuntu) to match the host's <user>:<group> names
## e.g. uid=1000(me), gid=1000(me)
COPY .env /tmp/.env
RUN . /tmp/.env && usermod -l "$USERNAME" -d /home/"$USERNAME" -m $(getent passwd "$UID" | cut -d: -f1) && \
  groupmod -n "$USERNAME" $(getent group "$GID" | cut -d: -f1)

WORKDIR /workspace

CMD ["/bin/bash"]
