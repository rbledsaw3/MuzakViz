#!/usr/bin/env sh

set -xe

CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

mkdir -p ./build
clang $CFLAGS -fPIC -shared -o ./build/libplug.so ./src/plug.c $LIBS
clang $CFLAGS -o ./build/muzakviz ./src/main.c $LIBS
