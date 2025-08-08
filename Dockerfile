FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    clang lld llvm llvm-dev \
    qemu-system-arm qemu-system-misc \
    build-essential make cmake ninja-build \
    python3 git gdb pkg-config \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
WORKDIR /work
