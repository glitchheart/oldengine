#!/bin/bash

WIGNORE="-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-old-style-cast -Wno-newline-eof -Wno-missing-prototypes -Wno-sign-conversion -Wno-shorten-64-to-32 -Wno-writable-strings -Wno-missing-variable-declarations"

pushd build

clang --std=c++14 -Weverything -fPIC $WIGNORE -shared -o libproject.so ../project_main.cpp 

popd
