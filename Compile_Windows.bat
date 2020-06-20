@echo off

if exist "build\" rd /s /q "build"

if not exist "build\" mkdir "build"
if not exist "release\" mkdir "release"

echo "Sto compilando il programma con le librerie..."
g++ -c main.cpp -o build\LIMPI-Windows.o -Ilib\xsocket -Ilib\xstring
g++ -c lib\xsocket\xsocket.cpp -o build\xsocket.o
g++ -c lib\xstring\xstring.c -o build\xstring.o

echo "Sto compilando le risorse..."
windres resources\icon.rc build\icon.o

echo "Sto compilando tutti i file oggetto..."
g++ build\LIMPI-Windows.o build\xsocket.o build\xstring.o build\icon.o -o release\LIMPI-Windows.exe -lws2_32

PAUSE