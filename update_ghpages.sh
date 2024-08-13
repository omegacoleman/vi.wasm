#!/bin/sh

rm -rf docs/index.html docs/assets
cp frontend/dist/index.html docs/
cp -R frontend/dist/assets docs/

