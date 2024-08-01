#!/bin/sh

pushd libc
make clean
make all -j20
popd

pushd vis
make clean
make all -j20 && cp ./vis.wasm ../frontend/vis.wasm
popd

