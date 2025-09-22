#!/bin/bash

SRC_DIR="src"
OUT="visicalc"

clear

g++ "$SRC_DIR"/*.cpp -o "$OUT"
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

./"$OUT"
