#! /bin/bash
rm -rf "build/"
rm -rf "release/"

mkdir "build"
mkdir "release"

echo "Sto compilando il programma con le librerie..."
g++ -c main.c -o build/LIMPI-Windows.o -Ilib/xsocket -Ilib/utils
g++ -c lib/xsocket/xsocket.c -o build/xsocket.o
g++ -c lib/utils/utils.c -o build/utils.o

echo "Sto compilando tutti i file oggetto..."
g++ build/LIMPI-Windows.o build/xsocket.o build/utils.o -o release/LIMPI-Linux