#!/bin/sh

pushd frontend
pnpm exec vite build
popd

