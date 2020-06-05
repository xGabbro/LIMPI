@echo off

if exist "build\" rd /s /q "build"
if exist "release\" rd /s /q "release"

if not exist "build\" mkdir "build"
if not exist "release\" mkdir "release"

echo "Sto compilando il programma con le librerie..."
g++ -c main.c -o build\LIMPI-Windows.o -Ilib\xsocket -Ilib\utils
g++ -c lib\xsocket\xsocket.c -o build\xsocket.o
g++ -c lib\utils\utils.c -o build\utils.o

echo "Sto compilando le risorse..."
windres resources\icon.rc build\icon.o

echo "Sto compilando tutti i file oggetto..."
g++ build\LIMPI-Windows.o build\xsocket.o build\utils.o build\icon.o -o release\LIMPI-Windows.exe -lws2_32

PAUSE