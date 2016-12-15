#!/bin/sh

NAME="p7zip_16.02"

rm -rf "$NAME"
tar -xvzf "../$NAME.tar.gz" >/dev/null 2>&1

cp ../makefile.machine "$NAME/"
cp ../MainAr.cpp "$NAME/CPP/7zip/UI/Console/MainAr.cpp"

cd "$NAME"

make -j 4 7zr

cp "bin/7zr" ../
