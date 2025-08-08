#!/usr/bin/env bash
set -e
IMAGE=aarch64-os-dev
docker build -t $IMAGE .
exec docker run --rm -it -v "$(pwd)":/work -w /work $IMAGE bash
