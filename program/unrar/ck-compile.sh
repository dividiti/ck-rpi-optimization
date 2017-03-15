#!/bin/sh

NAME="unrar"

rm -rf "$NAME"
tar -xvzf "../$NAME""src-5.4.5.tar.gz" >/dev/null 2>&1

cp ../makefile "$NAME/"
cp ../rar.cpp "$NAME/"

cd "$NAME"

make

cp "$NAME" ../a.out
