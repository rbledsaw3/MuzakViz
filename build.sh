#!/usr/bin/env sh

set -xe

CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

clang $CFLAGS -o muzakviz main.c $LIBS
