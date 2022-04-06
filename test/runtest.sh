#!/bin/sh

ROOT=$(dirname -- "${BASH_SOURCE[0]}")

g++ -Iinclude -std=c++20 "${ROOT}/test.cpp" -o ${ROOT}/test && ${ROOT}/test