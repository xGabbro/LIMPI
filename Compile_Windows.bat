@echo off
g++ main.c lib/xsocket/xsocket.c lib/utils/utils.c -o LIMPI-Windows.exe -Ilib/xsocket -Ilib/utils -lws2_32 
pause